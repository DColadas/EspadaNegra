#include "CardModel.hpp"

#include "Logging/Logger.hpp"

namespace Model {

const std::map<int, CardModel> CardModel::models = {
    {1, {"Inquisidor", 1, 0, 1, false}},
    {2, {"Campo", 2, 0, 1, false}},
    {3, {"Guerrero", 2, 1, 0, false}},
    {4, {"Caballero", 3, 0, 0, false}},
    {5, {"Ciudad", 0, 4, 0, false}},
    {6, {"Campesino", 0, 1, 2, false}},
    {7, {"Rey", 0, 0, 5, false}},
};

CardModel::CardModel(const std::string& name_,
                     int attack_,
                     int production_,
                     int victory_,
                     bool isBerserk_) : name(name_),
                                        attack(attack_),
                                        production(production_),
                                        victory(victory_),
                                        isBerserk(isBerserk_) {}

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