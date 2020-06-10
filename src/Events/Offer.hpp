#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerAction.hpp"
#include "Time.hpp"

// PlayerAction representing the amount of gold offered for the current card
class Offer final : public PlayerAction {
   protected:
    bool isEqual(const GameEvent& obj) const final {
        auto v = static_cast<const Offer&>(obj);
        return PlayerAction::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    Offer() : PlayerAction(){};

    Offer(const std::string& nickname,
          int gold)
        : PlayerAction(Type::Offer, nickname),
          gold(gold){};

    Offer(Timestamp time,
          const std::string& nickname,
          int gold)
        : PlayerAction(Type::Offer, time, nickname),
          gold(gold){};
};