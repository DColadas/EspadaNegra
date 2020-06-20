#pragma once

#include "Utils/Time.hpp"

// Represents any action sent by or to a player
class GameEvent {
   public:
    // To allow fast dispatch and parsing of GameEvents, Type contains a value
    // for every final GameEvent subclass
    enum class Type {
        Invalid,
        JoinMatchRequest,
        Draw,
        Attack,
        Pass,
        Offer,
        Earn,
        Pay,
        GetCard,
        Winner
        //TODO Add more
    };

   protected:
    Type type;

    // Valid GameEvent objects must only be created or moved from concrete subclases
    GameEvent(Type type_) : type(type_), time(Clock::now()){};
    GameEvent(Type type_, Timestamp time_) : type(type_), time(time_){};

    GameEvent(const GameEvent&) = default;
    GameEvent& operator=(const GameEvent&) = default;
    GameEvent(GameEvent&&) = default;
    GameEvent& operator=(GameEvent&&) = default;

    virtual bool isEqual(const GameEvent& o) const {
        return time == o.time;
    };

   public:
    Timestamp time;

    GameEvent() : type(Type::Invalid){};  //Only used when creating an invalid object
    virtual ~GameEvent() = default;

    // True if the object was created correctly
    inline bool isValid() const {
        return type != Type::Invalid;
    };

    // Returns the type of the current GameEvent (makes it read-only)
    inline Type getType() const {
        return type;
    }

    inline friend bool operator==(const GameEvent& lhs, const GameEvent& rhs) {
        return lhs.type != Type::Invalid &&
               rhs.type != Type::Invalid &&
               lhs.type == rhs.type &&
               lhs.isEqual(rhs);
    }

    inline friend bool operator!=(const GameEvent& lhs, const GameEvent& rhs) {
        return !(lhs == rhs);
    }
};
