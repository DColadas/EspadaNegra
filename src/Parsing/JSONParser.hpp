#pragma once

#include <memory>
#include <string>

#include "Utils/Time.hpp"

class InputEvent;
class OutputEvent;
namespace JSONParser {

// Returns unique_ptr to InputEvent parsed from the message
// Returns invalid InputEvent on error
// Valid input messages:
//  {"type": "joinMatchRequest", "matchID": "XXX", "nickname": "XXX"}
//  {"type": "attack"}
//  {"type": "pass"}
//  {"type": "offer", "gold": int}
[[nodiscard]] std::unique_ptr<InputEvent> messageToInputEvent(Timestamp time,
                                                              const std::string& nickname,
                                                              const std::string& message);

// Returns unique_ptr with the event in JSON
// Output messages:
//  {"type": "joinMatch", "nickname": "XXX"}
//  {"type": "attack", "nickname": "XXX"}
//  {"type": "offer", "nickname": "XXX", "gold": int}
//  {"type": "pass", "nickname": "XXX"}
//  {"type": "error", "message": "XXX"}
//  {"type": "pay", "nickname": "XXX", "gold": int}
//  {"type": "earn", "nickname": "XXX", "gold": int}
//  {"type": "winner", "nickname": "XXX"}
//  {"type": "getCard", "nickname": "XXX"}
//  {"type": "isAuctioneer", "nickname": "XXX"}
//  {"type": "leave", "nickname": "XXX", "reason": "XXX"}
//  {"type": "setGold", "nickname": "XXX", "gold": int}
//  {"type": "matchInfo", "deck": Deck, "players": [Player], "matchConfig": MatchConfig}
//  {"type": "draw", "card": Card}
//  {"type": "complex", "events": [Event]}
// where:
//  Card: {"id": int,
//         "name": "XXX",
//         "attack": int,
//         "production": int,
//         "victory": int,
//         "isBerserk": bool}
//  Deck: {"cards": [Card]}
//  Player: {"nickname": "XXX"}
//  MatchConfig: {"numPlayers": int}
//  Event: any other event object
[[nodiscard]] std::unique_ptr<const std::string> outputEventToMessage(const OutputEvent* event);

}  // namespace JSONParser
