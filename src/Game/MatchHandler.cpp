#include "MatchHandler.hpp"

#include <algorithm>

#include "Events/Error.hpp"
#include "Events/JoinMatchResult.hpp"
#include "Events/MatchInfo.hpp"
#include "IO/IOHandler.hpp"
#include "Logging/Logger.hpp"
#include "MatchConfig.hpp"
#include "Phase.hpp"
#include "Utils/Random.hpp"

MatchHandler::MatchHandler()
    : match(MatchConfig(2), Deck::getById(1)),
      maxPlayers(match.config.numPlayers) {
}

MatchHandler::MatchHandler(const MatchConfig& config, const Deck& deck)
    : match(std::move(config), std::move(deck)),
      maxPlayers(match.config.numPlayers) {
}

std::unique_ptr<const MatchInfo> MatchHandler::getMatchInfo() const {
    return std::make_unique<MatchInfo>(match.config,
                                       match.players,
                                       match.deck);
}

std::unique_ptr<const OutputEvent> MatchHandler::addPlayer(IOHandler* client,
                                                           const std::string& name) {
    if (isFull()) {
        const auto message = "Cannot join the match (full)";
        LOG_DEBUG(message);
        return std::make_unique<const Error>(message);
    } else if (isRunning()) {
        const auto message = "Cannot join the match (already running)";
        LOG_DEBUG(message);
        return std::make_unique<const Error>(message);
    }
    const auto inserted = handlers.emplace(name, client).second;
    if (!inserted) {
        // The nickname existed already
        const auto message = "Cannot join the match (nickname " + name + " already exists)";
        LOG_DEBUG(message);
        return std::make_unique<const Error>(message);
    }
    match.addPlayer(name);
    notifyPlayers(std::make_shared<const JoinMatchResult>(name));
    return getMatchInfo();
}

void MatchHandler::removePlayer(const std::string& nickname) {
    handlers.erase(nickname);
    match.removePlayer(nickname);
}

std::size_t MatchHandler::getPlayerCount() const {
    return handlers.size();
}

void MatchHandler::start() {
    notifyPlayers(match.start());
}

void MatchHandler::notifyPlayers(const std::shared_ptr<const OutputEvent>& event) {
    for (auto& h : handlers) {
        h.second->sendEvent(event);
    }
}

std::unique_ptr<const OutputEvent> MatchHandler::handleInputEvent(const InputEvent* action) {
    auto event = match.handleInputEvent(action);
    // If the input was invalid, send the error to the client who sent it
    if (event->isError()) {
        return std::move(event);
    }
    // If there is an update in the state of match, broadcast it
    std::shared_ptr<const OutputEvent> s = std::move(event);
    notifyPlayers(std::move(s));
    return nullptr;
}

bool MatchHandler::isRunning() const {
    return match.currentPhase != Phase::WaitForStart &&
           match.currentPhase != Phase::Finished;
}

bool MatchHandler::isFull() const {
    return maxPlayers == handlers.size();
}
