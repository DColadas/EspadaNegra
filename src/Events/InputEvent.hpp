#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <variant>

#include "Utils/Time.hpp"
namespace Events {

// Input event that contains necessary information to join a match
struct JoinMatchRequest {
    Timestamp time{Clock::now()};
    std::string nickname;
    std::string matchID;
};

// Input event representing the amount of gold offered for the current card
struct OfferRequest {
    Timestamp time{Clock::now()};
    std::string nickname;
    int gold;
};

// Input event by which the player is not interested in the current attack/auction phase
struct PassRequest {
    Timestamp time{Clock::now()};
    std::string nickname;
};

// Input event by which the player intents to attack the current card
struct AttackRequest {
    Timestamp time{Clock::now()};
    std::string nickname;
};

using InputEvent = std::variant<
    std::monostate,
    JoinMatchRequest,
    OfferRequest,
    PassRequest,
    AttackRequest>;

void from_json(const nlohmann::json& j, InputEvent& event) {
    enum class EventType {
        JoinMatchRequest_,
        OfferRequest_,
        PassRequest_,
        AttackRequest_,
        Invalid
    };

    using enum EventType;
    constexpr static auto toType = [&](std::string_view text) {
        if (text == "joinMatchRequest") {
            return JoinMatchRequest_;
        } else if (text == "offer") {
            return OfferRequest_;
        } else if (text == "pass") {
            return PassRequest_;
        } else if (text == "attack") {
            return AttackRequest_;
        } else
            return Invalid;
    };

    switch (toType(j.at("type").get<std::string_view>())) {
        case JoinMatchRequest_:
            event = JoinMatchRequest{
                .nickname = j.at("nickname"),
                .matchID = j.at("matchID")};
            break;
        case OfferRequest_:
            event = OfferRequest{
                .nickname = {},
                .gold = j.at("gold")};
            break;
        case PassRequest_:
            event = PassRequest{};
            break;
        case AttackRequest_:
            event = AttackRequest{};
            break;
        case Invalid:
            event = {};
            break;
    }
}

}  // namespace Events
