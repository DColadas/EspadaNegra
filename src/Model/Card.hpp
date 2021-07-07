#pragma once

#include <map>

#include "CardModel.hpp"
#include "GameModifier.hpp"
#include <nlohmann/json.hpp>

namespace Model {

class Card {
   private:
    static std::map<int, Card> cards;  //cardId, card

    Card(const CardModel* baseModel,
         int id,
         const GameModifier& initialCondition);

   public:
    const CardModel* base;
    int id;
    //TODO consider allowing more than one modifier
    GameModifier initialCondition;

    // Initialises the Card database
    // Must be run before any other Card method
    static void init();

    // Retrieve a Card by its id
    static Card getById(int id);

    // Get attributes
    std::string getName() const;
    int getAttack() const;
    int getProduction() const;
    int getVictory() const;
    bool isBerserk() const;

    bool operator==(const Card&) const;
    bool operator!=(const Card&) const;
};

void to_json(nlohmann::json&, const Card&);
void from_json(const nlohmann::json&, Card&);

}  // namespace Model