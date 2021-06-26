#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <variant>

#include "Utils/Time.hpp"

namespace Events {

// Event with a Timestamp.
// Only its derived structs should be instantiated.
struct TimedEvent {
    Timestamp time{Clock::now()};
};

// Event related to a certain player.
// Only its derived structs should be instantiated.
struct PlayerEvent {
    std::string nickname;
};

// Event representing an invalid action (malformed InputEvent, logic error...).
struct Error {
    std::string message;
};

// Event representing the amount of gold offered for the current card.
struct Offer: public PlayerEvent {
    int gold;
};

// Event by which the player is not interested in the current attack/auction phase.
struct Pass: public PlayerEvent {};

// Event by which the player intents to attack the current card.
struct Attack: public PlayerEvent {};

}  // namespace Events
