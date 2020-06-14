#pragma once

#include "GameModifier.hpp"

class MatchConfig {
   public:
    const unsigned int numPlayers;
    unsigned int cardsPerTurn = numPlayers + 2;
    unsigned int initialGold = 10;
    int auctioneerGoldDisadvantage = 1;

    MatchConfig(unsigned int numPlayers);

    // Changes current parameters according to the modifier
    void applyModifier(const GameModifier& modifier);
};
