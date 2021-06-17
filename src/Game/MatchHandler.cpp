#include "MatchHandler.hpp"

#include <algorithm>
#include <variant>

#include "Events/InputEvent.hpp"
#include "Events/OutputEvent.hpp"
#include "IO/IOHandler.hpp"
#include "Logging/Logger.hpp"
#include "Model/Deck.hpp"
#include "Model/MatchConfig.hpp"
#include "Phase.hpp"
#include "Utils/Random.hpp"
#include "Utils/Visitor.hpp"

MatchHandler::MatchHandler()
    : match(Model::MatchConfig(2), Model::Deck::getById(1)),
      maxPlayers(match.config.numPlayers) {
}

MatchHandler::MatchHandler(const Model::MatchConfig& config, const Model::Deck& deck)
    : match(std::move(config), std::move(deck)),
      maxPlayers(match.config.numPlayers) {
}

Events::MatchInfo MatchHandler::getMatchInfo() const {
    return Events::MatchInfo{match.config,
                             match.players,
                             match.deck};
}

Events::OutputEvent MatchHandler::addPlayer(IOHandler* client,
                                    const std::string& name) {
    if (isFull()) {
        const auto message = "Cannot join the match (full)";
        LOG_DEBUG(message);
        return Events::Error{message};
    } else if (isRunning()) {
        const auto message = "Cannot join the match (already running)";
        LOG_DEBUG(message);
        return Events::Error{message};
    }
    const auto inserted = handlers.emplace(name, client).second;
    if (!inserted) {
        // The nickname existed already
        const auto message = "Cannot join the match (nickname " + name + " already exists)";
        LOG_DEBUG(message);
        return Events::Error{message};
    }
    match.addPlayer(name);
    notifyPlayers(std::make_shared<const Events::OutputEvent>(Events::JoinMatchResult{name}));
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
    notifyPlayers(std::make_shared<const Events::OutputEvent>(match.start()));
}

void MatchHandler::notifyPlayers(const std::shared_ptr<const Events::OutputEvent>& event) {
    for (auto& h : handlers) {
        h.second->sendEvent(event);
    }
}

Events::OutputEvent MatchHandler::handleInputEvent(const Events::InputEvent& action) {
    auto event = match.handleInputEvent(action);
    return std::visit(visitor{
                          [&](const Events::Error& /**/) -> Events::OutputEvent {
                              // If the input was invalid, send the error to the client who sent it
                              return std::move(event);
                          },
                          [&](const auto& /**/) -> Events::OutputEvent {
                              // If there is an update in the state of match, broadcast it
                              notifyPlayers(std::make_shared<const Events::OutputEvent>(event));
                              return std::monostate{};
                          },
                      },
                      event);
}

bool MatchHandler::isRunning() const {
    return match.currentPhase != Phase::WaitForStart &&
           match.currentPhase != Phase::Finished;
}

bool MatchHandler::isFull() const {
    return maxPlayers == handlers.size();
}
