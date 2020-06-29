#include "Match.hpp"

#include <algorithm>

#include "Events/Attack.hpp"
#include "Events/Offer.hpp"
#include "Events/Pass.hpp"
#include "Logging/Logger.hpp"
#include "Utils/Random.hpp"

Match::Match(const MatchConfig& config_, const Deck& deck_)
    : config(std::move(config_)), deck(std::move(deck_)) {
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

void Match::resetAuctionWinners() {
    applyToPlayers([](Player& p) {
        p.isAuctionWinner = false;
    });
}

void Match::nextAuctioneer() {
    currentAuctioneer++;
    if (currentAuctioneer >= static_cast<int>(players.size())) {
        currentAuctioneer = 0;
    }
    currentPlayer = currentAuctioneer;
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
    unsigned int index;
    bool found = false;
    for (unsigned int i = 0; i < players.size(); ++i) {
        if (players[i].isAuctionWinner) {
            if (found) {
                // Two winners found, so no actual winner
                return std::nullopt;
            }
            found = true;
            index = i;
        }
    }
    if (found) {
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

void Match::start() {
    currentPhase = Phase::GameStart;
    processPhase();
}

std::unique_ptr<const GameEvent> Match::handlePlayerAction(const PlayerAction* action) {
    using GE = GameEvent::Type;
    auto retEvent = std::make_unique<const GameEvent>();
    const unsigned int index = getPlayerIndex(action->nickname);
    Player& p = players[index];
    switch (action->getType()) {
        case GE::Attack:
            // Only valid on Attack phase
            if (currentPhase != Phase::Attack) {
                break;
            }
            {
                const auto attack = static_cast<const Attack*>(action);
                if (p.canAttack(currentAttack)) {
                    const auto attackAmount = p.attack();
                    if (currentAttack < attackAmount) {
                        resetAuctionWinners();
                        currentAttack = attackAmount;
                    }
                    p.isAuctionWinner = true;
                    retEvent = std::make_unique<const Attack>(attack->nickname);
                }
            }
            break;

        case GE::Offer:
            // Only valid on Auction phase
            if (currentPhase != Phase::Auction) {
                break;
            }
            {
                const auto offer = static_cast<const Offer*>(action);
                const auto gold = offer->gold;
                if (currentOffer <= gold && p.canOffer(gold)) {
                    // If the new offer is higher, the current player is the new current winner
                    if (currentOffer < gold) {
                        resetAuctionWinners();
                        currentOffer = gold;
                    }
                    p.isAuctionWinner = true;
                    retEvent = std::make_unique<const Offer>(offer->nickname, offer->gold);
                }
            }
            break;

        case GE::Pass:
            // Only valid on Attack and Auction phases
            if (currentPhase != Phase::Attack && currentPhase != Phase::Auction) {
                break;
            }
            {
                const auto pass = static_cast<const Pass*>(action);
                p.pass();
                retEvent = std::make_unique<const Pass>(pass->nickname);
            }
            break;

        case GE::Invalid:
            LOG_ERROR("Invalid PlayerAction");
            break;
        default:
            LOG_PANIC("Unimplemented PlayerAction");
    }
    // If the input action was valid, update the state of the game
    if (retEvent->isValid()) {
        processPhase();
    }
    return retEvent;
}

void Match::onGameStartPhase() {
    // Decide a random auctioneer
    currentAuctioneer = Random::randInt(0, static_cast<int>(players.size()) - 1);
    currentPlayer = currentAuctioneer;

    // Set initial gold
    applyToPlayers([&](Player& p) {
        p.gold = static_cast<int>(config.initialGold);
    });
    players[static_cast<std::size_t>(currentAuctioneer)].gold -= config.auctioneerGoldDisadvantage;

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
        return;
    }

    // Set cards in auction
    for (unsigned int i = 0; i < config.cardsPerTurn; ++i) {
        table.add(deck.draw());
    }

    // Inform about current cards
    const auto auctionCards = table.getCards();
    for (const auto& c : auctionCards) {
        // TODO create event to inform the client about the cards
        LOG_TRACE("Card " + std::to_string(c.id) + ", " + c.getName());
    }

    // Every player earns their production
    applyToPlayers([&](Player& p) {
        p.earn(p.getTotalProduction());
    });

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
            LOG_TRACE(player.name + " gets the card on AttackPhase with attack " +
                      std::to_string(currentAttack));
            currentPhase = table.isEmpty() ? Phase::TurnEnd : Phase::Attack;
        } else {
            // There is a tie: auction the card
            LOG_TRACE("Tie for the card on AttackPhase (attack " +
                      std::to_string(currentAttack) + "): switch to AuctionPhase");
            currentPhase = Phase::Auction;
        }
    } else {
        // Nobody attacked: auction the card
        LOG_TRACE("No attacks on AttackPhase: switch to AuctionPhase");
        currentPhase = Phase::Auction;
    }

    // Reset control values
    applyToPlayers([&](Player& p) {
        p.hasPassed = false;
        p.isAuctionWinner = false;
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
            LOG_TRACE(player.name + " gets the card on AuctionPhase with offer " +
                      std::to_string(currentOffer));

        } else {
            // There is a tie
            // TODO normally, untying is done by the decision of an auctioneer
            // To reduce the complexity of the program for now, the card is
            // discarded on a tie.
            // The game should, instead, get into a Phase::AuctioneerDecision
            // and only process AuctioneerDecisions from the current one
            table.discard();
            LOG_TRACE("Tie for the card on AuctionPhase (offer " +
                      std::to_string(currentOffer) + "): switch to next card");
        }
    } else {
        // Nobody offered: discard
        LOG_TRACE("No offers on AuctionPhase: switch to next card");
        table.discard();
    }

    // Reset control values
    applyToPlayers([&](Player& p) {
        p.hasPassed = false;
        p.isAuctionWinner = false;
    });
    currentOffer = 0;
    currentPhase = table.isEmpty() ? Phase::TurnEnd : Phase::Attack;
    processPhase();
}

void Match::onTurnEndPhase() {
    LOG_TRACE("Entered TurnEnd");

    applyToPlayers([&](Player& p) {
        p.onTurnEnd();
    });

    // Change auctioneer
    nextAuctioneer();
}

void Match::onGameEndPhase() {
}
