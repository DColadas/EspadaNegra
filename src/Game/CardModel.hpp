#pragma once

#include <map>
#include <string>

class CardModel {
   private:
    //TODO should provide a set of models per match, to apply modifiers to them
    static std::map<int, CardModel> models;  //modelId, model
    CardModel(const std::string& name_,
              int attack_,
              int production_,
              int victory_,
              bool isBerserk_) : name(name_),
                                 attack(attack_),
                                 production(production_),
                                 victory(victory_),
                                 isBerserk(isBerserk_){};

   public:
    std::string name;
    int attack;
    int production;
    int victory;
    bool isBerserk;
    //  std::string image;

    static void add(int id,
                    const std::string& name,
                    int attack,
                    int production,
                    int victory,
                    bool isBerserk) {
        //TODO check models[id] does not exist already
        models[id] = CardModel(name, attack, production, victory, isBerserk);
    }

    static const CardModel& getById(int id) {
        //TODO if models[id] does not exist, this should create it
        //This is a problem, so check with a panic if the returned item is valid
        return models[id];
    }
};
