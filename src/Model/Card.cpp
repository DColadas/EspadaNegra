#include "Card.hpp"

#include <nlohmann/json.hpp>

#include "Logging/Logger.hpp"

namespace Model {

std::map<int, Card> Card::cards;

Card::Card(const CardModel* baseModel_,
           int id_,
           const GameModifier& initialCondition_) : base(baseModel_),
                                                    id(id_),
                                                    initialCondition(initialCondition_) {}

void Card::init() {
    std::map<int, int> cardAmount{
        // modelId, amount
        {1, 10},
        {2, 9},
        {3, 3},
        {4, 1},
        {5, 2},
        {6, 15},
        {7, 8},
    };
    int cardId = 1;
    for (const auto& [modelId, amount] : cardAmount) {
        for (int i = 0; i < amount; ++i) {
            cards.insert({cardId, {CardModel::getById(modelId), cardId, {}}});
            ++cardId;
        }
    }
}

std::string Card::getName() const {
    return base->name;
}

int Card::getAttack() const {
    return base->attack;
}

int Card::getProduction() const {
    return base->production;
}

int Card::getVictory() const {
    return base->victory;
}

bool Card::isBerserk() const {
    return base->isBerserk;
}

Card Card::getById(int id) {
    // TODO don't panic on invalid while the json parser uses getById
    const auto it = cards.find(id);
    LOG_PANIC_IF(it == cards.end(), "Card " + std::to_string(id) + " does not exist");
    return it->second;
}

bool Card::operator==(const Card& rhs) const {
    return id == rhs.id;
}
bool Card::operator!=(const Card& rhs) const {
    return !(*this == rhs);
}

void to_json(nlohmann::json& j, const Card& card) {
    j = nlohmann::json{{"id", card.id}};
}

void from_json(const nlohmann::json& j, Card& card) {
    card = Card::getById(j.at("id"));
}

}  // namespace Model