#pragma once

#include "GameModifier.hpp"

class MatchConfig {
   public:
    const int numPlayers;
    int cardsPerTurn = numPlayers + 2;
    int initialGold = 10;
    int auctioneerGoldDisadvantage = 1;

    MatchConfig(int numPlayers);

    // Changes current parameters according to the modifier
    void applyModifier(const GameModifier& modifier);
};
