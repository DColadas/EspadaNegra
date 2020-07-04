#pragma once

#include <string>

#include "PlayerResult.hpp"

// PlayerResult that sets a player's gold to a fixed amount
class SetGold final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const SetGold&>(obj);
        return PlayerResult::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    SetGold(const std::string& nickname_,
            int gold_)
        : PlayerResult(Type::SetGold, nickname_),
          gold(gold_) {}
};