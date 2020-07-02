#pragma once

#include <string>

#include "Utils/Time.hpp"

// Action sent by a player (not yet executed)
class InputEvent {
   public:
    // To allow fast dispatch and parsing of events, Type contains a value
    // for every final InputEvent subclass
    enum class Type {
        Invalid,
        JoinMatchRequest,
        AttackRequest,
        PassRequest,
        OfferRequest,
    };

   protected:
    Type type;

    InputEvent(Type type_,
               const std::string& nickname_)
        : type(type_), time(Clock::now()), nickname(nickname_) {}

    InputEvent(Type type_,
               Timestamp time_,
               const std::string& nickname_)
        : type(type_), time(time_), nickname(nickname_) {}

    InputEvent(const InputEvent&) = default;
    InputEvent& operator=(const InputEvent&) = default;
    InputEvent(InputEvent&&) = default;
    InputEvent& operator=(InputEvent&&) = default;

    virtual bool isEqual(const InputEvent& o) const {
        return time == o.time &&
               nickname == o.nickname;
    }

   public:
    Timestamp time;
    std::string nickname;

    InputEvent() : type(Type::Invalid){};  //Only used when creating an invalid object
    virtual ~InputEvent() = default;

    // True if the object was created correctly
    inline bool isValid() const {
        return type != Type::Invalid;
    };

    // Returns the type of the current InputEvent (makes it read-only)
    inline Type getType() const {
        return type;
    }

    inline friend bool operator==(const InputEvent& lhs, const InputEvent& rhs) {
        return lhs.type != Type::Invalid &&
               rhs.type != Type::Invalid &&
               lhs.type == rhs.type &&
               lhs.isEqual(rhs);
    }

    inline friend bool operator!=(const InputEvent& lhs, const InputEvent& rhs) {
        return !(lhs == rhs);
    }
};