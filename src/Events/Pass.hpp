#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerAction.hpp"
#include "Time.hpp"

// PlayerAction by which the player is not interested in the current attack/auction phase
class Pass final : public PlayerAction {
   protected:
    virtual bool isEqual(const GameEvent& obj) const override {
        auto v = static_cast<const Pass&>(obj);
        return PlayerAction::isEqual(obj);
    }

   public:
    Pass() : PlayerAction(){};

    Pass(const std::string& nickname)
        : PlayerAction(Type::Pass, nickname){};

    Pass(Timestamp time,
         const std::string& nickname)
        : PlayerAction(Type::Pass, time, nickname){};
};