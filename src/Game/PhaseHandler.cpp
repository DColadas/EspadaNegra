#include "PhaseHandler.hpp"

#include "Logging/Logger.hpp"
#include "Match.hpp"

PhaseHandler::PhaseHandler(Match& match_)
    : match(match_) {}

void PhaseHandler::processPhase() {
    switch (match.currentPhase) {
        case Phase::GameStart:
            match.onGameStartPhase();
            break;
        case Phase::TurnStart:
            match.onTurnStartPhase();
            break;
        case Phase::Attack:
            match.onAttackPhase();
            break;
        case Phase::Auction:
            match.onAuctionPhase();
            break;
        case Phase::TurnEnd:
            match.onTurnEndPhase();
            break;
        case Phase::GameEnd:
            match.onGameEndPhase();
            break;
        case Phase::WaitForStart:
        case Phase::Finished:
            LOG_PANIC("PhaseHandler called on Phase with no associated action");
            break;
        default:
            LOG_PANIC("Unsupported Phase");
            break;
    }
}
