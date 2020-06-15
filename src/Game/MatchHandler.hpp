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
    unsigned int maxPlayers;

    // Inform every player about the GameEvent
    void notifyPlayers(const std::shared_ptr<const GameEvent>& event);

   public:
    //TODO consider making private and using a factory to create Match

    // Creates a match of 2 people and deck 1
    MatchHandler();

    MatchHandler(const MatchConfig& config, const Deck& deck);

    // Add a new player to the match
    // True if the player was inserted
    // False if the nickname already exists, match is full or running
    bool addPlayer(IOHandler* client,
                   const std::string& nickname);

    // Remove player from the match
    void removePlayer(const std::string& nickname);

    // Returns the amount of connected players
    std::size_t getPlayerCount() const;

    // Start the game, process until the match ends
    void start();

    // Receive a PlayerAction from a client
    void handlePlayerAction(const PlayerAction& action);

    // True if the match is running
    bool isRunning() const;

    // True if the match is full (no more players can join)
    bool isFull() const;
};
