#pragma once

#include <string>

#include "PlayerResult.hpp"

// PlayerResult defining a player who won the match
class Winner final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const Winner&>(obj);
        return PlayerResult::isEqual(obj);
    }

   public:
    Winner(const std::string& nickname_)
        : PlayerResult(Type::Winner, nickname_){}
};