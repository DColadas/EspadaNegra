#pragma once

#include <map>

#include "CardModel.hpp"
#include "GameModifier.hpp"

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

    // Get attributes
    std::string getName() const;
    int getAttack() const;
    int getProduction() const;
    int getVictory() const;
    bool isBerserk() const;

    // Add a new card type which to retrieve later
    static void add(int id,
                    const GameModifier& initialCondition,
                    int modelId);
    static void add(int id,
                    const GameModifier& initialCondition,
                    const CardModel* model);

    // Returns first card if the id doesn't exist (should never happen)
    static Card getById(int id);
};
