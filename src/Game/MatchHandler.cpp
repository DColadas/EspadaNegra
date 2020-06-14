#include "MatchHandler.hpp"

#include <algorithm>

#include "IO/IOHandler.hpp"
#include "Utils/Random.hpp"

MatchHandler::MatchHandler(const MatchConfig& config, const Deck& deck)
    : match(std::move(config), std::move(deck)) {
}

void MatchHandler::addPlayer(IOHandler* client, const std::string& name) {
    handlers.emplace(name, client);
    match.addPlayer(name);
}

void MatchHandler::removePlayer(const std::string& nickname) {
    handlers.erase(nickname);
    match.removePlayer(nickname);
}

void MatchHandler::start() {
    match.start();
}

void MatchHandler::notifyPlayers(const GameEvent& event) {
    auto const e = std::make_shared<const GameEvent>(std::move(event));
    for (auto& h : handlers) {
        h.second->sendEvent(e);
    }
}

// Receive a PlayerAction from a client
void MatchHandler::handlePlayerAction(const PlayerAction& action) {
    const auto event = match.handlePlayerAction(action);
    // If there is an update in the state of match, broadcast it
    if (event) {
        notifyPlayers(*event);
    }
}
