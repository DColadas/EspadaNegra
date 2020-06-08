#pragma once

#include "Time.hpp"

// Represents any action sent by or to a player
class GameEvent {
   public:
    // To allow fast dispatch and parsing of GameEvents, Type contains a value
    // for every final GameEvent subclass
    enum class Type {
        JoinMatchRequest,
        Draw,
        Attack,
        Pass,
        Offer
        //TODO Add more
    };

   protected:
    bool valid;
    Type type;

    // Valid GameEvent objects must only be created or moved from concrete subclases
    GameEvent(Type type) : valid(true), type(type), time(Clock::now()){};
    GameEvent(Type type, Timestamp time) : valid(true), type(type), time(time){};

    GameEvent(const GameEvent&) = default;
    GameEvent& operator=(const GameEvent&) = default;
    GameEvent(GameEvent&&) = default;
    GameEvent& operator=(GameEvent&&) = default;

    virtual bool isEqual(const GameEvent& o) const {
        return time == o.time &&
               valid == o.valid;
    };

   public:
    Timestamp time;

    GameEvent() : valid(false){};  //Only used when creating an invalid object
    virtual ~GameEvent() = default;

    // True if the object was created correctly
    inline bool isValid() const {
        return valid;
    };

    // Returns the type of the current GameEvent (makes it read-only)
    inline Type getType() const {
        return type;
    }

    inline friend bool operator==(const GameEvent& lhs, const GameEvent& rhs) {
        return lhs.type == rhs.type && lhs.isEqual(rhs);
    }

    inline friend bool operator!=(const GameEvent& lhs, const GameEvent& rhs) {
        return !(lhs == rhs);
    }
};
