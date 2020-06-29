#pragma once

#include <deque>
#include <vector>

#include "Card.hpp"

class Table {
   private:
    std::deque<Card> cardsInAuction{};
    std::vector<Card> discarded{};

   public:
    Table() = default;

    // Adds a new card to the auction
    void add(Card card);

    // Draw the current card from the table
    Card pop();

    // True if no cards are left to auction on the table
    bool isEmpty() const;

    // Discards the current card
    void discard();
};
