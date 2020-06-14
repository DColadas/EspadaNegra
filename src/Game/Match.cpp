#include "Match.hpp"

#include <algorithm>

#include "Utils/Random.hpp"

Match::Match(const MatchConfig& config, const Deck& deck)
    : config(std::move(config)), deck(std::move(deck)) {
}

void Match::nextAuctioneer() {
    currentAuctioneer++;
    if (currentAuctioneer >= players.size()) {
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

void Match::start() {
    currentPhase = Phase::GameStart;
    while (currentPhase != Phase::Finished) {
        phaseHandler.processPhase();
    }
}

std::unique_ptr<const GameEvent> Match::handlePlayerAction(const PlayerAction& action) {
}

void Match::onGameStartPhase() {
    // Decide a random auctioneer
    currentAuctioneer = Random::randInt(0, players.size() - 1);
    currentPlayer = currentAuctioneer;

    //TODO draw 3 conditions and apply them
}

void Match::onTurnStartPhase() {
    // Put cards in the auction
    // If there are not enough cards left, end game
    if (!deck.canDraw(config.cardsPerTurn)) {
        currentPhase = Phase::GameEnd;
        return;
    }

    for (auto i = 0; i < config.cardsPerTurn; ++i) {
        cardsInAuction.push_back(deck.draw());
    }
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
