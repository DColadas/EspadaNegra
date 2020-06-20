#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerResult.hpp"
#include "Utils/Time.hpp"

// PlayerResult representing the amount of gold paid for the current card
class Pay final : public PlayerResult {
   protected:
    bool isEqual(const GameEvent& obj) const final {
        auto v = static_cast<const Pay&>(obj);
        return PlayerResult::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    Pay() : PlayerResult(){};

    Pay(const std::string& nickname_,
        int gold_)
        : PlayerResult(Type::Pay, nickname_),
          gold(gold_){};

    Pay(Timestamp time_,
        const std::string& nickname_,
        int gold_)
        : PlayerResult(Type::Pay, time_, nickname_),
          gold(gold_){};
};