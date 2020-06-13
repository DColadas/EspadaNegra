#pragma once

#include <string>

#include "PlayerEvent.hpp"
#include "Time.hpp"

// PlayerEvent representing an update of the state of a player
class PlayerResult : public PlayerEvent {
   protected:
    PlayerResult(Type type_,
                 const std::string& nickname_)
        : PlayerEvent(type_, nickname_){};

    PlayerResult(Type type_,
                 Timestamp time_,
                 const std::string& nickname_)
        : PlayerEvent(type_, time_, nickname_){};

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