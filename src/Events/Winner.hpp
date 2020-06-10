#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerResult.hpp"
#include "Time.hpp"

// PlayerResult defining a player who won the match
class Winner final : public PlayerResult {
   protected:
    bool isEqual(const GameEvent& obj) const final {
        auto v = static_cast<const Winner&>(obj);
        return PlayerResult::isEqual(obj);
    }

   public:
    Winner() : PlayerResult(){};

    Winner(const std::string& nickname)
        : PlayerResult(Type::Winner, nickname){};

    Winner(Timestamp time,
           const std::string& nickname)
        : PlayerResult(Type::Winner, time, nickname){};
};