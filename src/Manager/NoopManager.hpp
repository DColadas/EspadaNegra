#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "Game/MatchHandler.hpp"
#include "MatchManager.hpp"

class IOHandler;
namespace Events {
struct JoinMatchRequest;
}

// MatchManager whose methods do absolutely nothing
// Used for the client side
class NoopManager : public MatchManager {
   public:
    void join(IOHandler&) override{};
    void leave(IOHandler&) override{};
    void dispatchMessage(IOHandler&, std::string_view) override{};
    void broadcast(MatchHandler&,
                   const std::shared_ptr<const std::string>&) override{};
};
