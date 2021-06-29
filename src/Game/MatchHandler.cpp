#include "MatchHandler.hpp"

#include <algorithm>
#include <memory>
#include <variant>

#include "Events/InputEvent.hpp"
#include "Events/OutputEvent.hpp"
#include "Logging/Logger.hpp"
#include "Manager/MatchManager.hpp"
#include "Model/Deck.hpp"
#include "Model/MatchConfig.hpp"
#include "Phase.hpp"
#include "Utils/Random.hpp"
#include "Utils/Visitor.hpp"

MatchHandler::MatchHandler(MatchManager* manager_)
    : manager(manager_),
      match(Model::MatchConfig(2), Model::Deck::getById(1)),
      maxPlayers(match.config.numPlayers) {
}

MatchHandler::MatchHandler(MatchManager* manager_,
                           const Model::MatchConfig& config,
                           const Model::Deck& deck)
    : manager(manager_),
      match(std::move(config), std::move(deck)),
      maxPlayers(match.config.numPlayers) {
}

Events::MatchInfo MatchHandler::getMatchInfo() const {
    return Events::MatchInfo{match.config,
                             match.players,
                             match.deck};
}

Events::OutputEvent MatchHandler::addPlayer(const std::string& name) {
    if (isFull()) {
        const auto message = "Cannot join the match (full)";
        LOG_DEBUG(message);
        return Events::Error{message};
    } else if (isRunning()) {
        const auto message = "Cannot join the match (already running)";
        LOG_DEBUG(message);
        return Events::Error{message};
    }
    // Check if there's a player with the same nickname
    const auto& ps = match.players;
    const auto available = std::none_of(ps.begin(), ps.end(), [&](const auto& i) {
        return i == name;
    });
    if (!available) {
        // The nickname existed already
        const auto message = "Cannot join the match (nickname " + name + " already exists)";
        LOG_DEBUG(message);
        return Events::Error{message};
    }
    match.addPlayer(name);
    notifyPlayers(Events::JoinMatchResult{name});
    return getMatchInfo();
}

void MatchHandler::removePlayer(const std::string& nickname) {
    match.removePlayer(nickname);
}

std::size_t MatchHandler::getPlayerCount() const {
    return match.players.size();
}

void MatchHandler::start() {
    notifyPlayers(match.start());
}

void MatchHandler::notifyPlayers(const Events::OutputEvent& event) {
    manager->broadcast(*this, std::make_shared<const std::string>(Events::toMessage(event)));
}

Events::OutputEvent MatchHandler::handleEvent(const Events::InputEvent& action) {
    const auto event = std::visit([&](const auto& e) { return match.handleEvent(e); }, action);
    return std::visit(visitor{
                          [&](const Events::Error& /**/) -> Events::OutputEvent {
                              // If the input was invalid, send the error to the client who sent it
                              return event;
                          },
                          [&](const auto& /**/) -> Events::OutputEvent {
                              // If there is an update in the state of match, broadcast it
                              notifyPlayers(event);
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
    return maxPlayers == getPlayerCount();
}
