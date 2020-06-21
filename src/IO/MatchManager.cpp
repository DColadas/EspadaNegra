#include "MatchManager.hpp"

#include <thread>

#include "Events/JoinMatchRequest.hpp"
#include "Game/MatchHandler.hpp"
#include "Logging/Logger.hpp"

MatchManager::MatchManager() {
}

void MatchManager::join(IOHandler& handler) {
    handlers.insert(&handler);
}

void MatchManager::leave(IOHandler& handler) {
    handlers.erase(&handler);
}

void MatchManager::send(const std::string& message) {
    auto const ss = std::make_shared<const std::string>(std::move(message));

    for (auto handler : handlers)
        handler->sendMessage(ss);
}

MatchHandler* MatchManager::joinMatch(IOHandler& client, const JoinMatchRequest& req) {
    if (!req.isValid()) {
        return nullptr;
    }

    const auto& matchID = req.matchID;
    const auto& nickname = req.nickname;
    auto it = matches.find(matchID);
    if (it == matches.end()) {
        //No match with that ID exists, create match
        //TODO get iterator while inserting
        matches[matchID] = std::make_unique<MatchHandler>();
        it = matches.find(matchID);
        LOG_INFO("Match created: ID = " + matchID);
    } else {
        //Match found, join
    }
    if (!it->second->addPlayer(&client, nickname)) {
        // Couldn't join the match (full or existing username)
        LOG_DEBUG("Couldn't join " + matchID + " by " + nickname + " (full or existing username)");
        return nullptr;
    }

    LOG_DEBUG(nickname + " joins " + it->first + ", players " + std::to_string(it->second->getPlayerCount()));
    if (it->second->isFull()) {
        // If the match became full after joining, start it!
        //TODO starting threads for each match is far from ideal
        LOG_INFO("Start match " + matchID);
        std::thread([it] { it->second->start(); }).detach();
    }

    return it->second.get();
}
