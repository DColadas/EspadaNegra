#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "Events/GameEvent.hpp"
#include "Events/InputEvent.hpp"
#include "Events/JoinMatchRequest.hpp"

class IOHandler {
   public:
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
