#pragma once

#include "CardModel.hpp"
#include "GameModifier.hpp"

class Card {
   private:
    Card(const CardModel& baseModel_,
         int id_,
         const GameModifier& initialCondition_) : base(baseModel_),
                                                  id(id_),
                                                  initialCondition(initialCondition_){};

   public:
    const CardModel& base;
    int id;
    //TODO consider allowing more than one modifier
    GameModifier initialCondition;

    inline std::string getName() const {
        return base.name;
    }

    inline int getAttack() const {
        return base.attack;
    }

    inline int getProduction() const {
        return base.production;
    }

    inline int getVictory() const {
        return base.victory;
    }

    inline bool isBerserk() const {
        return base.isBerserk;
    }

    //static Card createById(int /*id*/) {
    //    //TODO
    //}
};
