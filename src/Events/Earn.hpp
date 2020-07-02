#pragma once

#include <string>

#include "PlayerResult.hpp"

// PlayerResult representing an amount of gold earned (for end of turn)
class Earn final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const Earn&>(obj);
        return PlayerResult::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    Earn(const std::string& nickname_,
         int gold_)
        : PlayerResult(Type::Earn, nickname_),
          gold(gold_) {}
};