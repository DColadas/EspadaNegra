#include "PhaseHandler.hpp"

#include "Match.hpp"

PhaseHandler::PhaseHandler(Match& match_)
    : match(match_) {}

void PhaseHandler::processPhase() {
    switch (match.currentPhase) {
        case Phase::WaitForStart:
        case Phase::Finished:
            // Do nothing (this method shouldn't even be called)
            //TODO add panic
            break;
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
        default:
            //TODO add panic
            break;
    }
}
