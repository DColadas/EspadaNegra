#pragma once

#include <string>

#include "PlayerResult.hpp"

// PlayerResult sent when a client leaves the Match
class Leave final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const Leave&>(obj);
        return PlayerResult::isEqual(obj) &&
               reason == v.reason;
    }

   public:
    std::string reason;

    Leave(const std::string& nickname_,
          const std::string& reason_)
        : PlayerResult(Type::Leave, nickname_),
          reason(reason_) {}
};