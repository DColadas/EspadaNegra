#pragma once

#include <vector>

#include "Card.hpp"

class Deck {
   private:
    std::vector<Card> cards;

    Deck();

   public:
    // Get the top card from the deck
    Card draw();

    // True if no cards are left
    bool isEmpty() const;

    // True if there are at least ${amount} cards left
    bool canDraw(int amount) const;

    // Shuffle the deck
    void shuffle();

    // Create a predefined deck by its ID (e.g. "Dormenia")
    static Deck createById(int id);
};
