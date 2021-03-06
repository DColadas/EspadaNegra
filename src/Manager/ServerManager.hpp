#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>

#include "Game/MatchHandler.hpp"
#include "MatchManager.hpp"

class IOHandler;
namespace Events {
struct JoinMatchRequest;
}

// Controls the communications between clients and matches and the open sessions
// Implementation used in the server side
class ServerManager : public MatchManager {
   public:
    void join(IOHandler& client) override;
    void leave(IOHandler& client) override;

    // Dispatch $message sent by $client
    void dispatchMessage(IOHandler& client, std::string_view message) override;

    // Sends $message to every IOHandler in $match
    void broadcast(MatchHandler& match,
                   const std::shared_ptr<const std::string>& message) override;

   private:
    // TODO this doesn't work if the io_context is run in many threads
    // Serialize access to this set using strands

    // matchID, MatchHandler
    std::unordered_map<std::string, std::unique_ptr<MatchHandler>> matches;

    // Shows which Match the IOHandler is playing and its nickname.
    // If the IOHandler is not playing, the pair is default constructed.
    std::unordered_map<IOHandler*, std::pair<MatchHandler*, std::string>> handlerMatch;

    // Shows which IOHandlers are in a Match.
    std::unordered_multimap<MatchHandler*, IOHandler*> matchHandlers;

    // Auxiliary method to join $client to the requested Match.
    void joinMatch(IOHandler& client, const Events::JoinMatchRequest& req);
};
