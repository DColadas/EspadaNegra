#pragma once

#include <string>

#include "OutputEvent.hpp"

// OutputEvent representing an update of the state of a player
class PlayerResult : public OutputEvent {
   protected:
    PlayerResult(Type type_,
                 const std::string& nickname_)
        : OutputEvent(type_), nickname(nickname_) {}

    PlayerResult(const PlayerResult&) = default;
    PlayerResult& operator=(const PlayerResult&) = default;
    PlayerResult(PlayerResult&&) = default;
    PlayerResult& operator=(PlayerResult&&) = default;

    bool isEqual(const OutputEvent& obj) const override {
        auto v = static_cast<const PlayerResult&>(obj);
        return OutputEvent::isEqual(obj) &&
               nickname == v.nickname;
    }

   public:
    std::string nickname;

    virtual ~PlayerResult() = default;
};