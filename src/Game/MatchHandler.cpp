#include "MatchHandler.hpp"

#include <algorithm>
#include <iostream>

#include "IO/IOHandler.hpp"
#include "MatchConfig.hpp"
#include "Utils/Random.hpp"

MatchHandler::MatchHandler()
    : match(MatchConfig(2), Deck::getById(1)),
      maxPlayers(match.config.numPlayers) {
}

MatchHandler::MatchHandler(const MatchConfig& config, const Deck& deck)
    : match(std::move(config), std::move(deck)),
      maxPlayers(match.config.numPlayers) {
}

void MatchHandler::addPlayer(IOHandler* client, const std::string& name) {
    handlers.emplace(name, client);
    match.addPlayer(name);
    if (handlers.size() == maxPlayers) {
        std::cout << "Start!" << std::endl;
    }
}

void MatchHandler::removePlayer(const std::string& nickname) {
    handlers.erase(nickname);
    match.removePlayer(nickname);
}

std::size_t MatchHandler::getPlayerCount() const {
    return handlers.size();
}

void MatchHandler::start() {
    match.start();
}

void MatchHandler::notifyPlayers(const std::shared_ptr<const GameEvent>& event) {
    for (auto& h : handlers) {
        h.second->sendEvent(event);
    }
}

// Receive a PlayerAction from a client
void MatchHandler::handlePlayerAction(const PlayerAction& action) {
    auto event = match.handlePlayerAction(action);
    // If there is an update in the state of match, broadcast it
    if (event) {
        std::shared_ptr<const GameEvent> s = std::move(event);
        notifyPlayers(std::move(s));
    }
}

bool MatchHandler::isRunning() const {
    return match.currentPhase != Phase::WaitForStart &&
           match.currentPhase != Phase::Finished;
}

bool MatchHandler::isFull() const {
    return maxPlayers == handlers.size();
}
