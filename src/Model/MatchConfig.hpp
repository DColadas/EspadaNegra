#pragma once

#include "GameModifier.hpp"
#include <nlohmann/json.hpp>

namespace Model {

class MatchConfig {
   public:
    unsigned int numPlayers;
    unsigned int cardsPerTurn = numPlayers + 2;
    unsigned int initialGold = 10;
    int auctioneerGoldDisadvantage = 1;

    MatchConfig() = default;
    explicit MatchConfig(unsigned int numPlayers);

    // Changes current parameters according to the modifier
    void applyModifier(const GameModifier& modifier);

    bool operator==(const MatchConfig&) const;
    bool operator!=(const MatchConfig&) const;
};

void to_json(nlohmann::json&, const MatchConfig&);
void from_json(const nlohmann::json&, MatchConfig&);

}  // namespace Model