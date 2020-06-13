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

    Attack(const std::string& nickname_)
        : PlayerAction(Type::Attack, nickname_){};

    Attack(Timestamp time_,
           const std::string& nickname_)
        : PlayerAction(Type::Attack, time_, nickname_){};
};