#pragma once

#include <string>
#include <vector>

#include "Card.hpp"

class Player {
   public:
    std::string name;
    int gold = 0;  // Set when match starts, with the MatchConfig value
    bool hasAttacked = false;
    bool hasPassed = false;
    bool isAuctionWinner = false;  // True if will get the current card
    std::vector<Card> activeCards{};
    std::vector<Card> purchasedThisTurnCards{};

    Player(const std::string& name);

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
    void addCard(const Card& card);

    // Sets ${hasAttacked} to true and returns the available attack amount
    int attack();

    // Sets ${hasPassed} to true (can't attack the current card or pay)
    void pass();

    // True if ${amount} is less or equal than the available gold and has not passed
    bool canOffer(int amount) const;

    // True if has equal or more ${amount} total attack and has not attacked or passed
    bool canAttack(int amount) const;
    // Same as ${canAttack(0)}
    bool canAttack() const;

    // Pay or earn ${amount} of gold
    void pay(int amount);
    void earn(int amount);
};
