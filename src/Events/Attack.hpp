#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerAction.hpp"
#include "Time.hpp"

// PlayerAction by which the player intents to attack the current card
class Attack final : public PlayerAction {
   protected:
    bool isEqual(const GameEvent& obj) const final {
        auto v = static_cast<const Attack&>(obj);
        return PlayerAction::isEqual(obj);
    }

   public:
    Attack() : PlayerAction(){};

    Attack(const std::string& nickname)
        : PlayerAction(Type::Attack, nickname){};

    Attack(Timestamp time,
           const std::string& nickname)
        : PlayerAction(Type::Attack, time, nickname){};
};