#pragma once

#include <deque>
#include <vector>

#include "Model/Card.hpp"

class Table {
   private:
    std::deque<Model::Card> cardsInAuction{};
    std::vector<Model::Card> discarded{};

   public:
    Table() = default;

    // Adds a new card to the auction
    void add(Model::Card card);

    // Draw the current card from the table
    Model::Card pop();

    // True if no cards are left to auction on the table
    bool isEmpty() const;

    // Discards the current card
    void discard();

    // Returns a vector with a view of the current cards
    std::vector<Model::Card> getCards() const;
};
