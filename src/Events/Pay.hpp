#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerResult.hpp"
#include "Time.hpp"

// PlayerResult representing the amount of gold paid for the current card
class Pay final : public PlayerResult {
   protected:
    virtual bool isEqual(const GameEvent& obj) const override {
        auto v = static_cast<const Pay&>(obj);
        return PlayerResult::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    Pay() : PlayerResult(){};

    Pay(const std::string& nickname,
        int gold)
        : PlayerResult(Type::Pay, nickname),
          gold(gold){};

    Pay(Timestamp time,
        const std::string& nickname,
        int gold)
        : PlayerResult(Type::Pay, time, nickname),
          gold(gold){};
};