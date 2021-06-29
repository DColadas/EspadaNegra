#pragma once

#include <map>
#include <string>

#include "Events/InputEvent.hpp"
#include "Events/OutputEvent.hpp"
#include "Match.hpp"

class MatchManager;
namespace Model {
class MatchConfig;
class Deck;
};  // namespace Model
class MatchHandler {
   public:
    // Creates a match of 2 people and deck 1
    MatchHandler(MatchManager* manager);

    MatchHandler(MatchManager* manager,
                 const Model::MatchConfig& config,
                 const Model::Deck& deck);

    // Add a new player to the match
    // Returns MatchInfo if successfully joined
    // Returs Error if nickname already exists, match is full or running
    Events::OutputEvent addPlayer(const std::string& nickname);

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
    Events::OutputEvent handleEvent(const Events::InputEvent& action);

    // True if the match is running
    bool isRunning() const;

    // True if the match is full (no more players can join)
    bool isFull() const;

   private:
    MatchManager* manager;
    Match match;
    unsigned int maxPlayers;

    // Returns MatchInfo from the current match
    Events::MatchInfo getMatchInfo() const;

    // Inform every player about the $event
    void notifyPlayers(const Events::OutputEvent& event);
};
