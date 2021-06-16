#include "Match.hpp"

#include <algorithm>

#include "Events/InputEvent.hpp"
#include "Events/OfferResult.hpp"
#include "Events/PassResult.hpp"
#include "Events/AttackResult.hpp"
#include "Events/Complex.hpp"
#include "Events/Draw.hpp"
#include "Events/Earn.hpp"
#include "Events/Error.hpp"
#include "Events/GetCard.hpp"
#include "Events/IsAuctioneer.hpp"
#include "Events/Pay.hpp"
#include "Events/SetGold.hpp"
#include "Events/Winner.hpp"
#include "Logging/Logger.hpp"
#include "Utils/Random.hpp"
#include "Utils/Visitor.hpp"

using OT = OutputEvent::Type;

Match::Match(const MatchConfig& config_, const Deck& deck_)
    : config(std::move(config_)), deck(std::move(deck_)) {
}

void Match::addEvent(std::unique_ptr<OutputEvent> event) {
    if (!updateEvent) {
        // No event: just move the new one
        updateEvent = std::move(event);
    } else if (updateEvent->getType() != OT::Complex) {
        // There was a simple event: create a complex one and add both to it
        auto complex = std::make_unique<Complex>();
        complex->add(std::move(updateEvent));
        complex->add(std::move(event));
        updateEvent = std::move(complex);
    } else {
        // The event is already Complex: just add the new one
        auto c = static_cast<Complex*>(updateEvent.get());
        c->add(std::move(event));
    }
}

void Match::setError(const std::string& message) {
    // If the InputEvent is invalid, there is no other game state update
    // Therefore, delete the old update
    updateEvent = std::make_unique<Error>(message);
}

unsigned int Match::getPlayerIndex(const std::string& nickname) const {
    const auto it = std::find_if(players.begin(), players.end(), [nickname](const Player& p) {
        return p.name == nickname;
    });
    const unsigned int index = static_cast<unsigned int>(std::distance(players.begin(), it));
    if (index < players.size()) {
        return index;
    }
    LOG_PANIC("Player of nickname " + nickname + " does not exist in match");
    // Although a panic prevents the function from getting here, there is
    // still a -Wreturn-type warning (so the return 0; is necessary)
    return 0;
}

void Match::applyToPlayers(std::function<void(Player&)> func) {
    std::for_each(players.begin(), players.end(), func);
}

void Match::setNewAuctionTie(Player& newTying) {
    applyToPlayers([](Player& p) {
        if (p.isAuctionWinner()) {
            p.setInAuctionTie();
        }
    });
    newTying.setInAuctionTie();
}

void Match::setNewAuctionWinner(Player& auctionWinner) {
    applyToPlayers([](Player& p) {
        if (p.isAuctionWinner() || p.isInAuctionTie()) {
            p.resetAuctionState();
        }
    });
    auctionWinner.setAuctionWinner();
}

void Match::nextAuctioneer() {
    currentAuctioneer++;
    if (currentAuctioneer >= static_cast<int>(players.size())) {
        currentAuctioneer = 0;
    }
    currentPlayer = currentAuctioneer;
    const auto& name = players[static_cast<std::size_t>(currentAuctioneer)].name;
    addEvent(std::make_unique<IsAuctioneer>(name));
    LOG_TRACE(name + " is the auctioneer");
}

bool Match::arePossibleAttacks(int amount) const {
    return std::any_of(players.begin(), players.end(), [amount](const Player& p) {
        return p.canAttack(amount);
    });
}

bool Match::arePossibleOffers(int amount) const {
    return std::any_of(players.begin(), players.end(), [amount](const Player& p) {
        return p.canOffer(amount);
    });
}

void Match::processPhase() {
    phaseHandler.processPhase();
}

bool Match::isThereWinner() const {
    return (currentPhase == Phase::Attack && currentAttack) ||
           (currentPhase == Phase::Auction && currentOffer);
}

std::optional<unsigned int> Match::getCurrentWinner() const {
    const auto it = std::find_if(players.begin(), players.end(), [](const Player& p) {
        return p.isAuctionWinner();
    });
    const unsigned int index = static_cast<unsigned int>(std::distance(players.begin(), it));
    if (index < players.size()) {
        return index;
    }
    return std::nullopt;
}

void Match::addPlayer(const std::string& name) {
    players.emplace_back(name);
}

void Match::removePlayer(const std::string& nickname) {
    //TODO adjust indexes of current auctioneer, player...
    auto p = std::find_if(players.begin(), players.end(), [&nickname](const Player& a) {
        return a.name == nickname;
    });
    if (p != players.end()) {
        players.erase(p);
    } else {
        LOG_PANIC("Player of nickname " + nickname + " does not exist in match");
    }
}

std::unique_ptr<const OutputEvent> Match::start() {
    currentPhase = Phase::GameStart;
    processPhase();
    return std::move(updateEvent);
}

std::unique_ptr<const OutputEvent> Match::handleInputEvent(const Events::InputEvent& action) {
    updateEvent = nullptr;
    std::visit(visitor{
                   [&](const Events::AttackRequest& attack) {
                       Player& p = players[getPlayerIndex(attack.nickname)];

                       // Only valid on Attack phase
                       if (currentPhase != Phase::Attack) {
                           const auto message = "Attack on non-attack phase (" + p.name + ")";
                           setError(message);
                           LOG_DEBUG(message);
                           return;
                       }
                       if (p.canAttack(currentAttack)) {
                           const auto attackAmount = p.attack();
                           if (currentAttack < attackAmount) {
                               // If attacking with higher attack than everyone, it's the only winner
                               setNewAuctionWinner(p);
                               currentAttack = attackAmount;
                               LOG_TRACE(p.name + " attacks, is current winner");
                           } else {
                               // Same attack, sets new tie
                               setNewAuctionTie(p);
                               LOG_TRACE(p.name + " attacks, sets tie");
                           }
                           addEvent(std::make_unique<AttackResult>(attack));
                       } else {
                           const auto message = "Invalid Attack (" + p.name + ")";
                           setError(message);
                           LOG_DEBUG(message);
                       }
                   },
                   [&](const Events::OfferRequest& offer) {
                       Player& p = players[getPlayerIndex(offer.nickname)];
                       // Only valid on Auction phase
                       if (currentPhase != Phase::Auction) {
                           const auto message = "Offer on non-offer phase (" + p.name + ")";
                           setError(message);
                           LOG_DEBUG(message);
                           return;
                       }
                       const auto gold = offer.gold;
                       if (gold > 0 && currentOffer <= gold && p.canOffer(gold)) {
                           if (currentOffer < gold) {
                               // If the new offer is higher, the current player is the new current winner
                               setNewAuctionWinner(p);
                               currentOffer = gold;
                               LOG_TRACE(p.name + " offers " + std::to_string(gold) + ", is current winner");
                           } else {
                               if (p.isInAuctionTie()) {
                                   // Cannot offer if it is already in a tie
                                   const auto message = "Invalid Offer (" + p.name + " was already in a tie)";
                                   setError(message);
                                   LOG_DEBUG(message);
                                   return;
                               }
                               // Same offer, sets new tie
                               setNewAuctionTie(p);
                               LOG_TRACE(p.name + " offers " + std::to_string(gold) + ", sets tie");
                           }
                           addEvent(std::make_unique<OfferResult>(offer));
                       } else {
                           const auto message = "Invalid Offer (" + p.name + " offered " + std::to_string(gold) + ")";
                           setError(message);
                           LOG_DEBUG(message);
                       }
                   },
                   [&](const Events::PassRequest& pass) {
                       Player& p = players[getPlayerIndex(pass.nickname)];
                       // Only valid on Attack and Auction phases
                       if (currentPhase != Phase::Attack && currentPhase != Phase::Auction) {
                           const auto message = "Pass on invalid phase (" + p.name + ")";
                           setError(message);
                           LOG_DEBUG(message);
                           return;
                       }
                       if (p.canPass()) {
                           p.pass();
                           addEvent(std::make_unique<PassResult>(pass));
                           LOG_TRACE(p.name + " passes");
                       } else {
                           const auto message = "Invalid Pass (" + p.name + ")";
                           setError(message);
                           LOG_DEBUG(message);
                       }
                   },
                   [&](const auto& /**/) {
                       const auto message = "Invalid action";
                       setError(message);
                       LOG_ERROR(message);
                   }},
               action);
    // If the input action was valid, update the state of the game
    LOG_PANIC_IF(!updateEvent, "No return OutputEvent was set");
    if (!updateEvent->isError()) {
        processPhase();
    }
    return std::move(updateEvent);
}

void Match::onGameStartPhase() {
    // Decide a random auctioneer
    currentAuctioneer = Random::randInt(0, static_cast<int>(players.size()) - 1);
    currentPlayer = currentAuctioneer;
    const auto& name = players[static_cast<std::size_t>(currentAuctioneer)].name;
    addEvent(std::make_unique<IsAuctioneer>(name));
    LOG_TRACE(name + " is the auctioneer");

    // Set initial gold
    applyToPlayers([&](Player& p) {
        p.gold = static_cast<int>(config.initialGold);
    });
    players[static_cast<std::size_t>(currentAuctioneer)].gold -= config.auctioneerGoldDisadvantage;
    for (const auto& p : players) {
        addEvent(std::make_unique<SetGold>(p.name, p.gold));
        LOG_TRACE(p.name + " has " + std::to_string(p.gold) + " gold");
    }

    //Shuffle the deck
    deck.shuffle();

    //TODO draw 3 conditions and apply them

    currentPhase = Phase::TurnStart;
    processPhase();
}

void Match::onTurnStartPhase() {
    // Put cards in the auction
    // If there are not enough cards left, end game
    if (!deck.canDraw(config.cardsPerTurn)) {
        LOG_TRACE("Not enough cards for last turn: switch to GameEnd");
        currentPhase = Phase::GameEnd;
        processPhase();
        return;
    }

    // Set cards in auction
    for (unsigned int i = 0; i < config.cardsPerTurn; ++i) {
        table.add(deck.draw());
    }

    // Inform about current cards
    const auto auctionCards = table.getCards();
    for (const auto& c : auctionCards) {
        addEvent(std::make_unique<Draw>(c));
        LOG_TRACE("Card " + std::to_string(c.id) + ", " + c.getName());
    }

    currentPhase = Phase::Attack;
    processPhase();
}

void Match::onAttackPhase() {
    if (arePossibleAttacks(currentAttack)) {
        // Attacks are still possible: wait until every player attacked, passed
        // or cannot do anything
        return;
    }
    if (isThereWinner()) {
        // Check the winner
        auto index = getCurrentWinner();
        if (index.has_value()) {
            // There is a winner: give them the card
            auto& player = players[index.value()];
            player.addCard(table.pop());
            currentPhase = table.isEmpty() ? Phase::TurnEnd : Phase::Attack;
            // Notify the players
            addEvent(std::make_unique<GetCard>(player.name));
            LOG_TRACE(player.name + " gets the card on AttackPhase with attack " +
                      std::to_string(currentAttack));
        } else {
            // There is a tie: auction the card
            // TODO add event
            LOG_TRACE("Tie for the card on AttackPhase (attack " +
                      std::to_string(currentAttack) + "): switch to AuctionPhase");
            currentPhase = Phase::Auction;
        }
    } else {
        // Nobody attacked: auction the card
        // TODO add event
        LOG_TRACE("No attacks on AttackPhase: switch to AuctionPhase");
        currentPhase = Phase::Auction;
    }

    // Reset control values
    applyToPlayers([](Player& p) {
        p.resetAuctionState();
    });
    currentAttack = 0;
    processPhase();
}

void Match::onAuctionPhase() {
    if (arePossibleOffers(currentOffer)) {
        // Offers are still possible: wait until every player offered, passed
        // or cannot do anything
        return;
    }
    if (isThereWinner()) {
        // Check the winner
        auto index = getCurrentWinner();
        if (index.has_value()) {
            // There is a winner: give them the card
            auto& player = players[index.value()];
            player.addCard(table.pop());
            player.pay(currentOffer);
            // Notify the players
            addEvent(std::make_unique<Pay>(player.name, currentOffer));
            addEvent(std::make_unique<GetCard>(player.name));
            LOG_TRACE(player.name + " gets the card on AuctionPhase with offer " +
                      std::to_string(currentOffer));
        } else {
            // There is a tie
            // TODO normally, untying is done by the decision of an auctioneer
            // To reduce the complexity of the program for now, the card is
            // discarded on a tie.
            // The game should, instead, get into a Phase::AuctioneerDecision
            // and only process AuctioneerDecisions from the current one
            // TODO add event
            table.discard();
            LOG_TRACE("Tie for the card on AuctionPhase (offer " +
                      std::to_string(currentOffer) + "): switch to next card");
        }
    } else {
        // Nobody offered: discard
        // TODO add event
        LOG_TRACE("No offers on AuctionPhase: switch to next card");
        table.discard();
    }

    // Reset control values
    applyToPlayers([](Player& p) {
        p.resetAuctionState();
    });
    currentOffer = 0;
    currentPhase = table.isEmpty() ? Phase::TurnEnd : Phase::Attack;
    processPhase();
}

void Match::onTurnEndPhase() {
    applyToPlayers([](Player& p) {
        p.onTurnEnd();
        p.earn(p.getTotalProduction());
    });
    for (const auto& p : players) {
        addEvent(std::make_unique<Earn>(p.name, p.getTotalProduction()));
    }

    // Change auctioneer (notified to players in the function)
    nextAuctioneer();

    currentPhase = Phase::TurnStart;
    processPhase();
}

void Match::onGameEndPhase() {
    // Check winner (or multiple winners)
    std::vector<unsigned int> winners;
    int maxVictory = players[0].getTotalVictory();
    for (unsigned int i = 0; i < players.size(); ++i) {
        const auto& p = players[i];
        const auto pVictory = p.getTotalVictory();
        if (pVictory >= maxVictory) {
            if (pVictory > maxVictory) {
                // Has more victory than everyone else: is only winner
                winners.clear();
            }
            winners.push_back(i);
        }
    }
    for (const auto& w : winners) {
        addEvent(std::make_unique<Winner>(players[w].name));
        LOG_DEBUG("Winner: " + players[w].name);
    }

    currentPhase = Phase::Finished;
    // TODO delete finished games (add FinishedMatchEvent, send to every player)
    // When every player has left the match, the MatchManager should automatically
    // delete it
}
