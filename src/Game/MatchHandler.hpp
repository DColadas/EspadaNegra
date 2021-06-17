#pragma once

#include <map>
#include <string>

#include "Events/InputEvent.hpp"
#include "Events/OutputEvent.hpp"
#include "Match.hpp"

class IOHandler;
namespace Model {
class MatchConfig;
class Deck;
};  // namespace Model
class MatchHandler {
   private:
    Match match;
    std::map<std::string, IOHandler*> handlers;  //nickname, handler
    unsigned int maxPlayers;

    // Inform every player about the OutputEvent
    void notifyPlayers(const std::shared_ptr<const Events::OutputEvent>& event);

    // Returns MatchInfo from the current match
    Events::MatchInfo getMatchInfo() const;

   public:
    //TODO consider making private and using a factory to create Match

    // Creates a match of 2 people and deck 1
    MatchHandler();

    MatchHandler(const Model::MatchConfig& config, const Model::Deck& deck);

    // Add a new player to the match
    // Returns MatchInfo if successfully joined
    // Returs Error if nickname already exists, match is full or running
    Events::OutputEvent addPlayer(IOHandler* client,
                                  const std::string& nickname);

    // Remove player from the match
    void removePlayer(const std::string& nickname);

    // Returns the amount of connected players
    std::size_t getPlayerCount() const;

    // Start the game
    // Notifies the players of the game state changes
    void start();

    // Receive an input event from a client
    // If there is a valid state update, returns nullptr
    // If the input event is invalid, returns a pointer with Error
    Events::OutputEvent handleInputEvent(const Events::InputEvent& action);

    // True if the match is running
    bool isRunning() const;

    // True if the match is full (no more players can join)
    bool isFull() const;
};
