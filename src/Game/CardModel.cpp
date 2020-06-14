#include "CardModel.hpp"

std::map<int, CardModel> CardModel::models;

CardModel::CardModel(const std::string& name_,
                     int attack_,
                     int production_,
                     int victory_,
                     bool isBerserk_) : name(name_),
                                        attack(attack_),
                                        production(production_),
                                        victory(victory_),
                                        isBerserk(isBerserk_) {}

void CardModel::add(int id,
                    const std::string& name,
                    int attack,
                    int production,
                    int victory,
                    bool isBerserk) {
    //TODO check models[id] does not exist already (shouldn't happen)
    models.emplace(id, CardModel(name, attack, production, victory, isBerserk));
}

const CardModel* CardModel::getById(int id) {
    const auto it = models.find(id);
    if (it != models.end()) {
        return &it->second;
    }
    // If the id does not exist, return nothing
    // Because it should not happen, TODO panic
    return nullptr;
}
