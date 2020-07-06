#pragma once

#include <string>

#include "JoinMatchRequest.hpp"
#include "PlayerResult.hpp"

// PlayerResult broadcasted when a valid JoinMatchRequest is received
class JoinMatchResult final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const JoinMatchResult&>(obj);
        return PlayerResult::isEqual(obj) &&
               matchID == v.matchID;
    }

   public:
    std::string matchID;

    JoinMatchResult(const std::string& nickname_)
        : PlayerResult(Type::JoinMatchResult, nickname_) {}

    // Does not check if the JoinMatchRequest is valid
    explicit JoinMatchResult(const JoinMatchRequest& o)
        : PlayerResult(Type::JoinMatchResult, o.nickname) {}
};