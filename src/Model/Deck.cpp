#include "Deck.hpp"

#include <algorithm>
#include <nlohmann/json.hpp>

#include "Logging/Logger.hpp"
#include "Utils/Random.hpp"

namespace Model {

std::map<int, Deck> Deck::decks;

Deck::Deck(const std::vector<int>& cardIds) {
    for (const auto& id : cardIds) {
        cards.push_back(Card::getById(id));
    }
}

Deck::Deck(const std::vector<Card>& cards_) : cards(cards_) {}

const std::vector<Card>& Deck::getCards() const {
    return cards;
}

Card Deck::draw() {
    LOG_PANIC_IF(isEmpty(), "Deck is empty");
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

void Deck::add(int id, const std::vector<int>& cardIds) {
    LOG_PANIC_IF(decks.find(id) != decks.end(),
                 "Deck " + std::to_string(id) + " already exists");
    decks.emplace(id, Deck(std::move(cardIds)));
}

void Deck::add(int id, const std::vector<Card>& cards_) {
    LOG_PANIC_IF(decks.find(id) != decks.end(),
                 "Deck " + std::to_string(id) + " already exists");
    decks.emplace(id, Deck(std::move(cards_)));
}

Deck Deck::getById(int id) {
    const auto it = decks.find(id);
    LOG_PANIC_IF(it == decks.end(), "Deck " + std::to_string(id) + " does not exist");
    return it->second;
}

bool Deck::operator==(const Deck& rhs) const {
    return cards == rhs.cards;
}

bool Deck::operator!=(const Deck& rhs) const {
    return !(*this == rhs);
}

void to_json(nlohmann::json& j, const Deck& deck) {
    j = nlohmann::json{{"cards", deck.getCards()}};
}

}  // namespace Model
