#pragma once

#include <string>

#include "InputEvent.hpp"

// InputEvent that contains necessary information to join a match
class JoinMatchRequest final : public InputEvent {
   protected:
    bool isEqual(const InputEvent& obj) const final {
        auto v = static_cast<const JoinMatchRequest&>(obj);
        return InputEvent::isEqual(obj) &&
               matchID == v.matchID;
    }

   public:
    std::string matchID;

    JoinMatchRequest() : InputEvent() {}

    JoinMatchRequest(const std::string& nickname_,
                     const std::string& matchID_)
        : InputEvent(Type::JoinMatchRequest, nickname_),
          matchID(matchID_) {}

    JoinMatchRequest(Timestamp time_,
                     const std::string& nickname_,
                     const std::string& matchID_)
        : InputEvent(Type::JoinMatchRequest, time_, nickname_),
          matchID(matchID_) {}
};