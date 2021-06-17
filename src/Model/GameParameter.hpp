#pragma once

namespace Model {

enum class GameParameter {
    Nothing,
    CardsPerTurn,
    OnlyVictoryCardsAdditionalVictory,
    CanProduceWithOnlyVictory,
    CanAttackWithOnlyVictory,
    PlayerWithMostEqualCardsGetsVictory,
    PlayerWithMostCardsIsAuctioneer
    //TODO add the rest
};

}  // namespace Model