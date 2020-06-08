#pragma once

enum class Phase {
    WaitForStart,
    GameStart,
    TurnStart,
    Attack,
    Auction,
    TurnEnd,
    GameEnd,
    Finished
};
