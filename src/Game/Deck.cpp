#include "Deck.hpp"

#include <algorithm>

#include "Utils/Random.hpp"

Card Deck::draw() {
    //TODO panic if no cards left
    const auto c = cards[cards.size() - 1];
    cards.pop_back();
    return c;
}

bool Deck::isEmpty() const {
    return cards.empty();
}

bool Deck::canDraw(unsigned int amount) const {
    return static_cast<unsigned int>(cards.size()) >= amount;
}

void Deck::shuffle() {
    Random::shuffle(cards.begin(), cards.end());
}

//Deck Deck::createById(int id) {
//    //TODO
//}