#pragma once

#include "GameParameter.hpp"

class GameModifier {
   public:
    GameParameter parameter;
    //TODO consider changing value to std::variant<int, bool>
    int value;

    GameModifier(GameParameter parameter, int value) : parameter(parameter),
                                                       value(value){};
};
