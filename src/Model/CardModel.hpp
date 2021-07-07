#pragma once

#include <map>
#include <string>

namespace Model {

class CardModel {
   private:
    //TODO should provide a set of models per match, to apply modifiers to them
    static const std::map<int, CardModel> models;  // modelId, model

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

    // Retrieves CardModel by its id
    static const CardModel* getById(int id);
};

}  // namespace Model