#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "Events/GameEvent.hpp"
#include "Events/JoinMatchRequest.hpp"
#include "Events/PlayerAction.hpp"

class IOHandler {
   public:
    bool isAlive = false;  // True if the connection is still valid
    bool inMatch = false;  // True if has already joined a match

    virtual void sendEvent(const std::shared_ptr<const GameEvent>& event) = 0;
    virtual void receiveMessage(const std::string& message) = 0;
    virtual void sendMessage(const std::shared_ptr<const std::string>& ss) = 0;

    IOHandler() = default;
    virtual ~IOHandler() = default;
    IOHandler(const IOHandler&) = delete;
    IOHandler& operator=(const IOHandler&) = delete;
    IOHandler(IOHandler&&) = delete;
    IOHandler& operator=(IOHandler&&) = delete;
};
