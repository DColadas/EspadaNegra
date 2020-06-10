#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "IOHandler.hpp"

// Represents the shared server state
class MatchManager {
   private:
    // TODO this doesn't work if the io_context is run in many threads
    // Serialize access to this set using strands
    std::unordered_set<IOHandler*> handlers;

    //matchID, Match (int for debugging purposes)
    std::unordered_map<std::string, std::unique_ptr<int>> matches;

   public:
    explicit MatchManager();

    void join(IOHandler& session);
    void leave(IOHandler& session);
    void send(const std::string& message);

    // Returns pointer to the Match where accepted
    // Returns nullptr if not accepted to the match
    int* joinMatch(const JoinMatchRequest& req);
};
