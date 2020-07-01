#pragma once

#include <string>
#include <vector>

#include "Card.hpp"

class Player {
   private:
    bool isWinningAuction = false;
    bool isTyingAuction = false;
    bool hasPassed = false;
    bool hasAttacked = false;

   public:
    std::string name;
    int gold = 0;  // Set when match starts, with the MatchConfig value
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

    // True if not ${isAuctionWinner}
    bool canPass() const;

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

    // Functions for the state in an auction
    // Current player is the only winner of the auction
    void setAuctionWinner();
    // Current player is tying the auction
    void setInAuctionTie();
    // Reset state regarding the auctioned card
    // Use on transition between Attack and Auction or cards
    void resetAuctionState();

    bool isAuctionWinner() const;
    bool isInAuctionTie() const;
};
