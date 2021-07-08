#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "Card.hpp"

namespace Model {

class Deck {
   private:
    static std::map<int, Deck> decks;  //deckId, deck

    std::vector<Card> cards;

   public:
    Deck() = default;
    explicit Deck(const std::vector<int>& cardIds);
    explicit Deck(const std::vector<Card>& cards);

    // Gets a view to the cards in the deck
    const std::vector<Card>& getCards() const;

    // Get the top card from the deck
    Card draw();

    // True if no cards are left
    bool isEmpty() const;

    // True if there are at least ${amount} cards left
    bool canDraw(unsigned int amount) const;

    // Shuffle the deck
    void shuffle();

    // Add a new deck type which to retrieve later
    static void add(int id, const std::vector<int>& cardIds);
    static void add(int id, const std::vector<Card>& cards);

    // Create a predefined deck by its ID
    static Deck getById(int id);

    bool operator==(const Deck& rhs) const;
    bool operator!=(const Deck& rhs) const;
};

void to_json(nlohmann::json&, const Deck&);
void from_json(const nlohmann::json&, Deck&);

}  // namespace Model
