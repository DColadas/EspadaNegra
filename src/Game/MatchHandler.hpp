#pragma once

#include <map>
#include <string>

#include "Match.hpp"

class IOHandler;
class GameEvent;
class PlayerAction;
class MatchHandler {
   private:
    Match match;
    std::map<std::string, IOHandler*> handlers;  //nickname, handler

    // Inform every player about the GameEvent
    void notifyPlayers(const std::shared_ptr<const GameEvent>& event);

   public:
    //TODO consider making private and using a factory to create Match
    MatchHandler(const MatchConfig& config, const Deck& deck);

    // Add a new player to the match
    void addPlayer(IOHandler* client,
                   const std::string& nickname);

    // Remove player from the match
    void removePlayer(const std::string& nickname);

    // Returns the amount of connected players
    std::size_t getPlayerCount() const;

    // Start the game, process until the match ends
    void start();

    // Receive a PlayerAction from a client
    void handlePlayerAction(const PlayerAction& action);
};
