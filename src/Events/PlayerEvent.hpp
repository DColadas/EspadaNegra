#pragma once

#include <string>

#include "GameEvent.hpp"
#include "Utils/Time.hpp"

// GameEvent related to the state of a player
class PlayerEvent : public GameEvent {
   protected:
    PlayerEvent(Type type_,
                const std::string& nickname_)
        : GameEvent(type_),
          nickname(nickname_){};

    PlayerEvent(Type type_,
                Timestamp time_,
                const std::string& nickname_)
        : GameEvent(type_, time_),
          nickname(nickname_){};

    PlayerEvent(const PlayerEvent&) = default;
    PlayerEvent& operator=(const PlayerEvent&) = default;
    PlayerEvent(PlayerEvent&&) = default;
    PlayerEvent& operator=(PlayerEvent&&) = default;

    bool isEqual(const GameEvent& obj) const override {
        auto v = static_cast<const PlayerEvent&>(obj);
        return GameEvent::isEqual(obj) &&
               nickname == v.nickname;
    }

   public:
    std::string nickname;

    PlayerEvent() : GameEvent(){};

    virtual ~PlayerEvent() = default;
};