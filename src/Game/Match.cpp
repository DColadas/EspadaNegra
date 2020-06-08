#include "Match.hpp"

#include <algorithm>

#include "Utils/Random.hpp"

Match::Match(const MatchConfig& config, const Deck& deck)
    : config(config), deck(deck) {
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

void Match::addPlayer(std::unique_ptr<IOHandler>&& connection,
                      const std::string& name) {
    players.push_back(Player(*this, std::move(connection), name));
}

void Match::start() {
    currentPhase = Phase::GameStart;
    while (currentPhase != Phase::Finished) {
        phaseHandler.processPhase();
    }
}

void Match::notifyPlayers(const GameEvent& event) {
    for (auto& p : players) {
        p.sendEvent(event);
    }
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
        //IDEA getAction() to every player that can attack
        // When any input arrived, update and reset the timer,
        // and ask everyone for action again.
        // If PASS is received, dont update
        //use future.wait_until

        //Make it so the IOHandler is constantly listening for events,
        //and update the ${latestAction} whenever a valid one is received

        //When getAction() is called, set ${latestAction} to null (or similar),
        //or just wait for an update

        //Or even, set a flag in the IOHandler to enable reception or not, so that
        //events received while not getAction() are not consuming resources
    while(arePossibleAttacks()){

    }
}

void Match::onAuctionPhase() {
}

void Match::onTurnEndPhase() {
    cardsInAuction.clear();
}

void Match::onGameEndPhase() {
}
