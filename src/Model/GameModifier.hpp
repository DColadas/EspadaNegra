#pragma once

#include "GameParameter.hpp"

namespace Model {

class GameModifier {
   public:
    GameParameter parameter;
    //TODO consider changing value to std::variant<int, bool>
    int value;

    GameModifier() : parameter(GameParameter::Nothing) {}
    GameModifier(GameParameter parameter_, int value_) : parameter(parameter_),
                                                         value(value_){};
};

}  // namespace Model