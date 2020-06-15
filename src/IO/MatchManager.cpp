#include "MatchManager.hpp"

#include <iostream>
#include <thread>

#include "Events/JoinMatchRequest.hpp"
#include "Game/MatchHandler.hpp"

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
        std::cout << "Match created - ";
    } else {
        //Match found, join
        std::cout << "Match found - ";
    }
    if (!it->second->addPlayer(&client, nickname)) {
        // Couldn't join the match (full or existing username)
        return nullptr;
    }

    std::cout << it->first << "::" << it->second->getPlayerCount() << std::endl;
    if (it->second->isFull()) {
        // If the match became full after joining, start it!
        //TODO starting threads for each match is far from ideal
        std::thread([it] { it->second->start(); }).detach();
    }

    return it->second.get();
}
