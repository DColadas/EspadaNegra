#include "Card.hpp"

#include "Logging/Logger.hpp"

std::map<int, Card> Card::cards;

Card::Card(const CardModel* baseModel_,
           int id_,
           const GameModifier& initialCondition_) : base(baseModel_),
                                                    id(id_),
                                                    initialCondition(initialCondition_) {}

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

void Card::add(int id,
               const GameModifier& initialCondition,
               const CardModel* model) {
    LOG_PANIC_IF(cards.find(id) != cards.end(),
                 "Card " + std::to_string(id) + " already exists");
    cards.emplace(id, Card(model, id, std::move(initialCondition)));
}

void Card::add(int id,
               const GameModifier& initialCondition,
               int modelId) {
    const auto model = CardModel::getById(modelId);
    add(id, initialCondition, model);
}

Card Card::getById(int id) {
    const auto it = cards.find(id);
    LOG_PANIC_IF(it == cards.end(), "Card " + std::to_string(id) + " does not exist");
    return it->second;
}
