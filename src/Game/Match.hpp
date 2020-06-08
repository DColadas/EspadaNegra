#pragma once

#include <vector>

#include "Card.hpp"
#include "Deck.hpp"
#include "MatchConfig.hpp"
#include "Phase.hpp"
#include "PhaseHandler.hpp"
#include "Player.hpp"

class Match {
   private:
    // Change the ${currentAuctioneer} index
    void nextAuctioneer();
    // True if some player has ${amount} or more attack and can attack
    bool arePossibleAttacks(int amount) const;
    
   public:
    Phase currentPhase = Phase::WaitForStart;
    PhaseHandler phaseHandler = PhaseHandler(*this);
    std::vector<Card> cardsInAuction{};
    std::vector<Card> discarded{};
    MatchConfig config;
    Deck deck;
    std::vector<Player> players{};
    int currentAuctioneer = -1;  // index in ${players}
    int currentPlayer = -1;      // index in ${players}

    //TODO consider making private and using a factory
    Match(const MatchConfig& config, const Deck& deck);

    // Add a new player to the match
    void addPlayer(std::unique_ptr<IOHandler>&& connection,
                   const std::string& name);

    // Start the game, process until the match ends
    void start();

    // Inform every player about the GameEvent
    void notifyPlayers(const GameEvent& event);

    // Methods for each Phase in the game
    void onGameStartPhase();
    void onTurnStartPhase();
    void onAttackPhase();
    void onAuctionPhase();
    void onTurnEndPhase();
    void onGameEndPhase();
};
