#pragma once

#include <memory>
#include <vector>

#include "Card.hpp"
#include "Deck.hpp"
#include "Events/GameEvent.hpp"
#include "Events/PlayerAction.hpp"
#include "MatchConfig.hpp"
#include "Phase.hpp"
#include "PhaseHandler.hpp"
#include "Player.hpp"

class Match {
   private:
    // Get player index by nickname
    unsigned int getPlayerIndex(const std::string& nickname) const;
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
    void addPlayer(const std::string& name);

    // Remove player from the match
    void removePlayer(const std::string& nickname);

    // Start the game, process until the match ends
    void start();

    // Process PlayerAction and return GameEvent with update of the Match's state
    // Returns invalid event if there is no valid update in the state
    std::unique_ptr<const GameEvent> handlePlayerAction(const PlayerAction* action);

    // Methods for each Phase in the game
    void onGameStartPhase();
    void onTurnStartPhase();
    void onAttackPhase();
    void onAuctionPhase();
    void onTurnEndPhase();
    void onGameEndPhase();
};
