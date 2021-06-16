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

}  // namespace Events
