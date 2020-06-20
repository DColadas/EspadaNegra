#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerResult.hpp"
#include "Utils/Time.hpp"

// PlayerResult representing an amount of gold earned (for end of turn)
class Earn final : public PlayerResult {
   protected:
    bool isEqual(const GameEvent& obj) const final {
        auto v = static_cast<const Earn&>(obj);
        return PlayerResult::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    Earn() : PlayerResult(){};

    Earn(const std::string& nickname_,
         int gold_)
        : PlayerResult(Type::Earn, nickname_),
          gold(gold_){};

    Earn(Timestamp time_,
         const std::string& nickname_,
         int gold_)
        : PlayerResult(Type::Earn, time_, nickname_),
          gold(gold_){};
};