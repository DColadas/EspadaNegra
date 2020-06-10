#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerResult.hpp"
#include "Time.hpp"

// PlayerResult representing an amount of gold earned (for end of turn)
class Earn final : public PlayerResult {
   protected:
    virtual bool isEqual(const GameEvent& obj) const override {
        auto v = static_cast<const Earn&>(obj);
        return PlayerResult::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    Earn() : PlayerResult(){};

    Earn(const std::string& nickname,
         int gold)
        : PlayerResult(Type::Earn, nickname),
          gold(gold){};

    Earn(Timestamp time,
         const std::string& nickname,
         int gold)
        : PlayerResult(Type::Earn, time, nickname),
          gold(gold){};
};