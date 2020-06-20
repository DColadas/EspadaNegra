#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerAction.hpp"
#include "Utils/Time.hpp"

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

    Offer(const std::string& nickname_,
          int gold_)
        : PlayerAction(Type::Offer, nickname_),
          gold(gold_){};

    Offer(Timestamp time_,
          const std::string& nickname_,
          int gold_)
        : PlayerAction(Type::Offer, time_, nickname_),
          gold(gold_){};
};