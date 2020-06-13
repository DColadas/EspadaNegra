#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerAction.hpp"
#include "Time.hpp"

// PlayerAction by which the player is not interested in the current attack/auction phase
class Pass final : public PlayerAction {
   protected:
    bool isEqual(const GameEvent& obj) const final {
        auto v = static_cast<const Pass&>(obj);
        return PlayerAction::isEqual(obj);
    }

   public:
    Pass() : PlayerAction(){};

    Pass(const std::string& nickname_)
        : PlayerAction(Type::Pass, nickname_){};

    Pass(Timestamp time_,
         const std::string& nickname_)
        : PlayerAction(Type::Pass, time_, nickname_){};
};