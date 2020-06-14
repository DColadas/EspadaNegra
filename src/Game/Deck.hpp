#pragma once

#include <map>
#include <vector>

#include "Card.hpp"

class Deck {
   private:
    static std::map<int, Deck> decks;  //deckId, deck

    std::vector<Card> cards;

    Deck(const std::vector<int>& cardIds);
    Deck(const std::vector<Card>& cards);

   public:
    // Get the top card from the deck
    Card draw();

    // True if no cards are left
    bool isEmpty() const;

    // True if there are at least ${amount} cards left
    bool canDraw(unsigned int amount) const;

    // Shuffle the deck
    void shuffle();

    static Deck createById(int id);

    // Add a new deck type which to retrieve later
    static void add(int id, const std::vector<int>& cardIds);
    static void add(int id, const std::vector<Card>& cards);

    // Create a predefined deck by its ID
    static Deck getById(int id);
};
