#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "Game/MatchHandler.hpp"

class IOHandler;
namespace Events {
struct JoinMatchRequest;
}

// Controls the communications between clients and matches and the open sessions
class MatchManager {
   public:
    virtual void join(IOHandler& client) = 0;
    virtual void leave(IOHandler& client) = 0;

    // Dispatch $message sent by $client
    virtual void dispatchMessage(IOHandler& client, std::string_view message) = 0;

    // Sends $message to every player in $match
    virtual void broadcast(MatchHandler& match,
                           const std::shared_ptr<const std::string>& message) = 0;

    virtual ~MatchManager() = default;
};
