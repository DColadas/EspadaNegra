#pragma once

#include <string>

#include "PlayerEvent.hpp"
#include "Time.hpp"

// PlayerEvent representing an intended action sent by a player (not yet executed)
class PlayerAction : public PlayerEvent {
   protected:
    PlayerAction(Type type_,
                 const std::string& nickname_)
        : PlayerEvent(type_, nickname_){};

    PlayerAction(Type type_,
                 Timestamp time_,
                 const std::string& nickname_)
        : PlayerEvent(type_, time_, nickname_){};

    PlayerAction(const PlayerAction&) = default;
    PlayerAction& operator=(const PlayerAction&) = default;
    PlayerAction(PlayerAction&&) = default;
    PlayerAction& operator=(PlayerAction&&) = default;

    bool isEqual(const GameEvent& obj) const override {
        auto v = static_cast<const PlayerAction&>(obj);
        return PlayerEvent::isEqual(obj);
    }

   public:
    PlayerAction() : PlayerEvent(){};

    virtual ~PlayerAction() = default;
};