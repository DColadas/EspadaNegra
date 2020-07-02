#pragma once

#include <string>

#include "PlayerResult.hpp"

// PlayerResult representing the amount of gold paid for the current card
class Pay final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const Pay&>(obj);
        return PlayerResult::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    Pay(const std::string& nickname_,
        int gold_)
        : PlayerResult(Type::Pay, nickname_),
          gold(gold_) {}
};