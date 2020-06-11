#pragma once

#include <string>

#include "PlayerEvent.hpp"
#include "Time.hpp"

// PlayerEvent representing an update of the state of a player
class PlayerResult : public PlayerEvent {
   protected:
    PlayerResult(Type type,
                 const std::string& nickname)
        : PlayerEvent(type, nickname){};

    PlayerResult(Type type,
                 Timestamp time,
                 const std::string& nickname)
        : PlayerEvent(type, time, nickname){};

    PlayerResult(const PlayerResult&) = default;
    PlayerResult& operator=(const PlayerResult&) = default;
    PlayerResult(PlayerResult&&) = default;
    PlayerResult& operator=(PlayerResult&&) = default;

    bool isEqual(const GameEvent& obj) const override {
        auto v = static_cast<const PlayerResult&>(obj);
        return PlayerEvent::isEqual(obj);
    }

   public:
    PlayerResult() : PlayerEvent(){};

    virtual ~PlayerResult() = default;
};