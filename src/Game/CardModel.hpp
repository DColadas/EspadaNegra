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
              bool isBerserk);

   public:
    std::string name;
    int attack;
    int production;
    int victory;
    bool isBerserk;
    //  std::string image;

    // Add a new model which to retrieve later
    static void add(int id,
                    const std::string& name,
                    int attack,
                    int production,
                    int victory,
                    bool isBerserk);

    // Returns nullptr if the id doesn't exist (should never happen)
    static const CardModel* getById(int id);
};
