#pragma once

#include <string>
#include <string_view>
#include <variant>

#include "Utils/Time.hpp"

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