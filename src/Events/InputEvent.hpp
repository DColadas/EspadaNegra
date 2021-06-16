#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <variant>

#include "Events/Common.hpp"

namespace Events {

// Input event that contains necessary information to join a match.
struct JoinMatchRequest {
    std::string nickname;
    std::string matchID;
};

// Input event representing the amount of gold offered for the current card.
struct OfferRequest : public TimedEvent, public PlayerEvent {
    int gold;
};

// Input event by which the player is not interested in the current attack/auction phase.
struct PassRequest : public TimedEvent, public PlayerEvent {};

// Input event by which the player intents to attack the current card.
struct AttackRequest : public TimedEvent, public PlayerEvent {};

using InputEvent = std::variant<
    std::monostate,
    JoinMatchRequest,
    OfferRequest,
    PassRequest,
    AttackRequest>;

// Function used by nlohmann::json (ignore)
void from_json(const nlohmann::json& j, InputEvent& event);

// Parse InputEvent from $message and bind $nickname to it if PlayerEvent.
// Throws nlohmann::json::exception on malformed $message.
// Returns std::monostate on wrong type messages.
// Valid input messages:
//  {"type": "joinMatchRequest", "matchID": "XXX", "nickname": "XXX"}
//  {"type": "attack"}
//  {"type": "pass"}
//  {"type": "offer", "gold": int}
[[nodiscard]] InputEvent toInputEvent(std::string_view nickname,
                                      std::string_view message);

}  // namespace Events
