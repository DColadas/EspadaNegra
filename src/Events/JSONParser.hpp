#pragma once

#include <memory>
#include <string>

#include "GameEvent.hpp"
#include "Time.hpp"

namespace JSONParser {

// Returns unique_ptr to GameEvent parsed from the message
// Returns invalid GameEvent on error
// Valid input messages:
//  {"type": "joinMatchRequest", "matchID": "XXX", "nickname": "XXX"}
//  {"type": "attack"}
//  {"type": "pass"}
//  {"type": "offer", "gold": int}
[[nodiscard]] std::unique_ptr<GameEvent> messageToGameEvent(Timestamp time, const std::string& nickname, const std::string& message);

// Returns unique_ptr with the event in JSON
// If the event is not valid, returns nullptr
// Output messages:
//  {"type": "pay", "nickname": "XXX", "gold": int}
//  {"type": "earn", "nickname": "XXX", "gold": int}
//  {"type": "winner", "nickname": "XXX"}
//  {"type": "getCard", "nickname": "XXX"}
[[nodiscard]] std::unique_ptr<const std::string> gameEventToMessage(const GameEvent* event);

}  // namespace JSONParser
