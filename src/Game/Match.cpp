#include "Match.hpp"

#include <algorithm>

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
    } else {
        //TODO player not in the vector (should never happen)
    }
    abort();
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
        //Should never happen
        //TODO panic
    }
}

void Match::start() {
    currentPhase = Phase::GameStart;
    while (currentPhase != Phase::Finished) {
        phaseHandler.processPhase();
    }
}

std::unique_ptr<const GameEvent> Match::handlePlayerAction(const PlayerAction& action) {
    //TODO implement
}

void Match::onGameStartPhase() {
    // Decide a random auctioneer
    currentAuctioneer = Random::randInt(0, static_cast<int>(players.size()) - 1);
    currentPlayer = currentAuctioneer;

    // Set initial gold
    std::for_each(players.begin(), players.end(), [&](Player& p) {
        p.gold = static_cast<int>(config.initialGold);
    });
    players[static_cast<std::size_t>(currentAuctioneer)].gold -= config.auctioneerGoldDisadvantage;

    //TODO draw 3 conditions and apply them

    currentPhase = Phase::TurnStart;
}

void Match::onTurnStartPhase() {
    // Put cards in the auction
    // If there are not enough cards left, end game
    if (!deck.canDraw(config.cardsPerTurn)) {
        currentPhase = Phase::GameEnd;
        return;
    }

    // Set cards in auction
    for (unsigned int i = 0; i < config.cardsPerTurn; ++i) {
        cardsInAuction.push_back(deck.draw());
    }

    // Every player earns their production
    std::for_each(players.begin(), players.end(), [&](Player& p) {
        p.earn(p.getTotalProduction());
    });

    // Change auctioneer
    nextAuctioneer();

    currentPhase = Phase::Attack;
}

void Match::onAttackPhase() {
    // When any input arrived, update and reset the timer,
    // If PASS is received, dont update
    //while (arePossibleAttacks()) {
    //}
}

void Match::onAuctionPhase() {
}

void Match::onTurnEndPhase() {
    cardsInAuction.clear();
}

void Match::onGameEndPhase() {
}
