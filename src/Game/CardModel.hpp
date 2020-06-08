#pragma once

#include <map>
#include <string>

class CardModel {
   private:
   //TODO should provide a set of models per match, to apply modifiers to them
    static std::map<int, CardModel> models;  //modelId, model
    CardModel(const std::string& name,
              int attack,
              int production,
              int victory,
              bool isBerserk) : name(name),
                                attack(attack),
                                production(production),
                                victory(victory),
                                isBerserk(isBerserk){};

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
