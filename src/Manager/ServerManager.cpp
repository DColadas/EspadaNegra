#include "ServerManager.hpp"

#include <variant>

#include "Events/InputEvent.hpp"
#include "Events/OutputEvent.hpp"
#include "Game/MatchHandler.hpp"
#include "IO/IOHandler.hpp"
#include "Logging/Logger.hpp"
#include "Utils/Visitor.hpp"

namespace {
void sendEvent(IOHandler& client, const Events::OutputEvent event) {
    client.sendMessage(std::make_shared<const std::string>(Events::toMessage(event)));
}
}  // namespace

void ServerManager::join(IOHandler& handler) {
    handlerMatch.insert({&handler, {}});
}

void ServerManager::leave(IOHandler& handler) {
    if (const auto& it = handlerMatch.find(&handler);
        it != handlerMatch.end()) {
        const auto& [match, nickname] = it->second;
        // Delete handler from every helper container
        if(!match) {
            return;
        }
        match->removePlayer(nickname);
        const auto range = matchHandlers.equal_range(match);
        for (auto i = range.first; i != range.second; ++i) {
            if (i->second == &handler) {
                matchHandlers.erase(i);
                break;
            }
        }
    }
}

void ServerManager::broadcast(MatchHandler& match,
                             const std::shared_ptr<const std::string>& message) {
    const auto handlers = matchHandlers.equal_range(&match);
    for (auto i = handlers.first; i != handlers.second; ++i) {
        i->second->sendMessage(message);
    }
}

void ServerManager::dispatchMessage(IOHandler& client, std::string_view message) {
    // TODO check if client exists in the map? I don't think it's needed
    const auto& [currentMatch, nickname] = handlerMatch.find(&client)->second;
    const auto& curr = currentMatch;  // Quick fix for clang :)
    auto event = Events::toInputEvent(nickname, message);
    // If the event is Error, report it
    if (std::holds_alternative<Events::Error>(event)) {
        sendEvent(client, std::get<Events::Error>(event));
        return;
    }
    // TODO nickname check disable until a login action is implemented
    if (/*!nickname.empty() && */ !currentMatch) {
        // Not in match
        std::visit(
            visitor{
                [&](const Events::JoinMatchRequest& jmr) {
                    LOG_TRACE("JoinMatchRequest received");
                    joinMatch(client, jmr);
                },
                [&](const auto& /**/) {
                    LOG_ERROR("Event not implemented when not in match");
                },
            },
            event);
    } else {
        // In match
        std::visit(
            visitor{
                [&]<class T>(const T& pe) requires std::is_base_of_v<Events::PlayerEvent, T> {
                    LOG_TRACE("User " + pe.nickname + ": valid InputEvent received");
                    const auto response = curr->handleEvent(event);
                    // There is a specific message for this client (an Error)
                    if (std::holds_alternative<Events::Error>(response)) {
            sendEvent(client, response);
                    }
    }
    ,
        [&](const auto& /**/) {
            LOG_ERROR("Event not implemented when in match");
        },
}, event);
}
}

void ServerManager::joinMatch(IOHandler& client, const Events::JoinMatchRequest& req) {
    const auto& matchID = req.matchID;
    const auto& nickname = req.nickname;
    auto it = matches.find(matchID);
    if (it == matches.end()) {
        //No match with that ID exists, create match
        //TODO get iterator while inserting
        matches[matchID] = std::make_unique<MatchHandler>(this);
        it = matches.find(matchID);
        LOG_INFO("Match created: ID = " + matchID);
    } else {
        //Match found, join
    }
    const auto& match = it->second;  // Alias
    const auto response = match->addPlayer(nickname);
    std::visit(visitor{
                   [&](const Events::Error& err) {
                       // Couldn't join the match (notify the error)
                       LOG_DEBUG("Couldn't join " + matchID + " by " + nickname + " (full or existing username): " + err.message);
                       sendEvent(client, response);
                   },
                   [&](const auto& /**/) {
                       // Register the client in the maps
                       matchHandlers.emplace(match.get(), &client);
                       handlerMatch[&client] = std::make_pair(match.get(), nickname);
                       // Send MatchInfo
                       sendEvent(client, response);
                       LOG_DEBUG(nickname + " joins " + it->first + ", players " + std::to_string(match->getPlayerCount()));
                       if (match->isFull()) {
                           // If the match became full after joining, start it!
                           LOG_INFO("Start match " + matchID);
                           match->start();
                       }
                   },
               },
               response);
}
