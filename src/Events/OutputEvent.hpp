#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <vector>

#include "Events/Common.hpp"
#include "Events/InputEvent.hpp"
#include "Model/Card.hpp"
#include "Model/Deck.hpp"
#include "Model/MatchConfig.hpp"
#include "Model/Player.hpp"

namespace Events {

// Composite OutputEvent, formed by many others
struct Complex;

// Event that informs about a new card in the auction.
struct Draw {
    Model::Card card;
};

// PlayerEvent representing an amount of gold earned (for end of turn).
struct Earn : public PlayerEvent {
    int gold;
};

// PlayerEvent defining which player obtained the current auctioned card.
struct GetCard : public PlayerEvent {};

// PlayerEvent informing about who is the current auctioneer.
struct IsAuctioneer : public PlayerEvent {};

// PlayerEvent broadcasted when a valid JoinMatchRequest is received.
struct JoinMatchResult : public PlayerEvent {
    explicit JoinMatchResult(const std::string& nickname_)
        : PlayerEvent{nickname_} {}

    explicit JoinMatchResult(const JoinMatchRequest& o)
        : PlayerEvent{o.nickname} {}
};

// PlayerEvent sent when a client leaves the Match.
struct Leave : public PlayerEvent {
    std::string reason;
};

// Event sent to every player who just joined the match.
struct MatchInfo {
    // TODO keep an eye on this: huge size!
    // I think const& wouldn't work ($deck may change before it's parsed).
    Model::MatchConfig config;
    std::vector<Model::Player> players;
    Model::Deck deck;
};

// PlayerEvent representing the amount of gold paid for the current card.
struct Pay : public PlayerEvent {
    int gold;
};

// PlayerEvent that sets a player's gold to a fixed amount.
struct SetGold : public PlayerEvent {
    int gold;
};

// PlayerEvent defining the player who won the match.
struct Winner : public PlayerEvent {};

// Event sent to the players to inform about the state of the Match.
using OutputEvent = std::variant<
    std::monostate,
    Error,
    Attack,
    Complex,
    Draw,
    Earn,
    GetCard,
    IsAuctioneer,
    JoinMatchResult,
    Leave,
    MatchInfo,
    Offer,
    Pass,
    Pay,
    SetGold,
    Winner>;

// Definition of Complex (above).
struct Complex {
    std::vector<OutputEvent> events;

    Complex& operator+=(const OutputEvent& rhs) {
        events.push_back(rhs);
        return *this;
    }
};

// Here comes a clusterduck of boilerplate! Could be improved with templates.
void to_json(nlohmann::json& j, const Attack& event);
void to_json(nlohmann::json& j, const Complex& event);
void to_json(nlohmann::json& j, const Draw& event);
void to_json(nlohmann::json& j, const Earn& event);
void to_json(nlohmann::json& j, const Error& event);
void to_json(nlohmann::json& j, const GetCard& event);
void to_json(nlohmann::json& j, const IsAuctioneer& event);
void to_json(nlohmann::json& j, const JoinMatchResult& event);
void to_json(nlohmann::json& j, const Leave& event);
void to_json(nlohmann::json& j, const MatchInfo& event);
void to_json(nlohmann::json& j, const Offer& event);
void to_json(nlohmann::json& j, const Pass& event);
void to_json(nlohmann::json& j, const Pay& event);
void to_json(nlohmann::json& j, const SetGold& event);
void to_json(nlohmann::json& j, const Winner& event);
void to_json(nlohmann::json& j, const OutputEvent& event);

// Returns $event in JSON.
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
[[nodiscard]] std::string toMessage(const OutputEvent& event);

}  // namespace Events
