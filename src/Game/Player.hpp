#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Card.hpp"
#include "Match.hpp"

class Player {
   private:
    std::unique_ptr<IOHandler> connection;
    const Match& match;

   public:
    std::string name;
    int gold = 0;  // Set when match starts, with the MatchConfig value
    bool hasAttacked = false;
    std::vector<Card> activeCards{};
    std::vector<Card> purchasedThisTurnCards{};

    Player(const Match& match,
           std::unique_ptr<IOHandler>&& connection,
           const std::string& name);

    // Add the attack of every active attack card
    // Doesn't consider ${hasAttacked} or ${purchasedThisTurnCards}
    int getTotalAttack() const;

    // Add the production of every card
    int getTotalProduction() const;

    // Add the victory of every card
    int getTotalVictory() const;

    // Set the variables for next turn
    void onTurnEnd();

    // Add a new card to the owned ones
    void addCard(Card card);

    // Sets ${hasAttacked} to true and returns the available attack amount
    int attack();

    // True if ${amount} is less or equal than the available gold
    bool canOffer(int amount) const;

    // True if there are active attack cards and not ${hasAttacked}
    bool canAttack() const;
    // True if ${canAttack()} and has equal or more ${amount} total attack
    bool canAttack(int amount) const;

    // Pay or earn ${amount} of gold
    void pay(int amount);
    void earn(int amount);

    // Get user's latest valid PlayerAction
    PlayerAction getAction();

    // Notify player of a GameEvent
    void sendEvent(const GameEvent& event);
};
