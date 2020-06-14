#include "Card.hpp"

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
               int modelId) {
    const auto model = CardModel::getById(modelId);
    if (model) {
        //TODO check cards[id] does not exist already (shouldn't happen)
        cards.emplace(id, Card(model, id, std::move(initialCondition)));
    } else {
        //TODO panic
    }
}

void Card::add(int id,
               const GameModifier& initialCondition,
               const CardModel* model) {
    //TODO check cards[id] does not exist already (shouldn't happen)
    cards.emplace(id, Card(model, id, std::move(initialCondition)));
}

Card Card::getById(int id) {
    const auto it = cards.find(id);
    if (it != cards.end()) {
        return it->second;
    }
    //TODO panic
    return cards.begin()->second;
}
