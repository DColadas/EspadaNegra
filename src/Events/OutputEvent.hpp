#pragma once

#include <string>

// Event generated as a result of an OutputEvent (update on game state)
class OutputEvent {
   public:
    // To allow fast dispatch and parsing of events, Type contains a value
    // for every final OutputEvent subclass
    enum class Type {
        Error,
        Draw,
        MatchInfo,
        GetCard,
        Winner,
        Pay,
        Earn,
        JoinMatchResult,
        PassResult,
        AttackResult,
        OfferResult,
        IsAuctioneer,
        Leave,
        SetGold,
        Complex,
    };

   protected:
    Type type;

    OutputEvent(Type type_)
        : type(type_) {}

    OutputEvent(const OutputEvent&) = default;
    OutputEvent& operator=(const OutputEvent&) = default;
    OutputEvent(OutputEvent&&) = default;
    OutputEvent& operator=(OutputEvent&&) = default;

    virtual bool isEqual(const OutputEvent&) const {
        // Although there are no attributes, this isEqual should still be called in
        // derived isEqual, in case attributes (like timestamp) are added.
        // Therefore, this is not a pure virtual function
        return true;
    }

   public:
    virtual ~OutputEvent() = default;

    // True if the OutputEvent is an error (only information for the sending
    // client, not a game state update)
    constexpr bool isError() const {
        return type == Type::Error;
    }

    // Returns the type of the current OutputEvent (makes it read-only)
    constexpr Type getType() const {
        return type;
    }

    inline friend bool operator==(const OutputEvent& lhs, const OutputEvent& rhs) {
        return lhs.type == rhs.type &&
               lhs.isEqual(rhs);
    }

    inline friend bool operator!=(const OutputEvent& lhs, const OutputEvent& rhs) {
        return !(lhs == rhs);
    }
};