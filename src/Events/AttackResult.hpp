#pragma once

#include <string>

#include "InputEvent.hpp"
#include "PlayerResult.hpp"

// PlayerResult broadcasted when a valid AttackRequest is received
class AttackResult final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const AttackResult&>(obj);
        return PlayerResult::isEqual(obj);
    }

   public:
    AttackResult(const std::string& nickname_)
        : PlayerResult(Type::AttackResult, nickname_) {}

    // Does not check if the AttackRequest is valid
    explicit AttackResult(const Events::AttackRequest& o)
        : PlayerResult(Type::AttackResult, o.nickname) {}
};