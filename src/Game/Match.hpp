#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "Card.hpp"
#include "Deck.hpp"
#include "Events/GameEvent.hpp"
#include "Events/InputEvent.hpp"
#include "MatchConfig.hpp"
#include "Phase.hpp"
#include "PhaseHandler.hpp"
#include "Player.hpp"
#include "Table.hpp"

class Match {
   private:
    int currentAttack = 0;  // Max attack used for the current card
    int currentOffer = 0;   // Max gold offered for the current card

    // Get player index by nickname
    unsigned int getPlayerIndex(const std::string& nickname) const;

    // Applies ${func} to every player
    void applyToPlayers(std::function<void(Player&)> func);

    // Sets every auction winner to tying
    void setNewAuctionTie(Player& newTying);

    // Sets every auction winner or in tie to not winner or tiying
    void setNewAuctionWinner(Player& auctionWinner);

    // Change the ${currentAuctioneer} index
    void nextAuctioneer();

    // True if some player has ${amount} or more attack and can attack
    bool arePossibleAttacks(int amount) const;

    // True if some player has ${amount} or more gold and can offer
    bool arePossibleOffers(int amount) const;

    // Carry out computations regarding the current state of the game
    void processPhase();

    // True if some player offered or attacked the current card
    bool isThereWinner() const;

    // Determines the index of the current winner of the phase
    // Returns std::nullopt if there is no winner
    std::optional<unsigned int> getCurrentWinner() const;

   public:
    Phase currentPhase = Phase::WaitForStart;
    PhaseHandler phaseHandler = PhaseHandler(*this);
    Table table{};
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

    // Process InputEvent and return GameEvent with update of the Match's state
    // Returns invalid event if there is no valid update in the state
    std::unique_ptr<const GameEvent> handleInputEvent(const InputEvent* action);

    // Methods for each Phase in the game
    void onGameStartPhase();
    void onTurnStartPhase();
    void onAttackPhase();
    void onAuctionPhase();
    void onTurnEndPhase();
    void onGameEndPhase();
};
