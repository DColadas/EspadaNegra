#pragma once

#include <string>

#include "GameEvent.hpp"
#include "Time.hpp"

// GameEvent related to the state of a player
class PlayerEvent : public GameEvent {
   protected:
    PlayerEvent(Type type,
                const std::string& nickname)
        : GameEvent(type),
          nickname(nickname){};

    PlayerEvent(Type type,
                Timestamp time,
                const std::string& nickname)
        : GameEvent(type, time),
          nickname(nickname){};

    PlayerEvent(const PlayerEvent&) = default;
    PlayerEvent& operator=(const PlayerEvent&) = default;
    PlayerEvent(PlayerEvent&&) = default;
    PlayerEvent& operator=(PlayerEvent&&) = default;

    virtual bool isEqual(const GameEvent& obj) const override {
        auto v = static_cast<const PlayerEvent&>(obj);
        return GameEvent::isEqual(obj) &&
               nickname == v.nickname;
    }

   public:
    std::string nickname;

    PlayerEvent() : GameEvent(){};

    virtual ~PlayerEvent() = default;
};