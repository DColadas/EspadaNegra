#include "CardModel.hpp"

#include "Logging/Logger.hpp"

namespace Model {

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
    LOG_PANIC_IF(models.find(id) != models.end(),
                 "CardModel " + std::to_string(id) + " already exists");
    models.emplace(id, CardModel(name, attack, production, victory, isBerserk));
}

const CardModel* CardModel::getById(int id) {
    const auto it = models.find(id);
    if (it != models.end()) {
        return &it->second;
    }
    LOG_PANIC("CardModel " + std::to_string(id) + " does not exist");
    // Although a panic prevents the function from getting here, there is
    // still a -Wreturn-type warning (so the return 0; is necessary)
    return nullptr;
}

}  // namespace Model