#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>

class OutputEvent;
class IOHandler {
   public:
    virtual void sendEvent(const std::shared_ptr<const OutputEvent>& event) = 0;
    virtual void receiveMessage(const std::string& message) = 0;
    virtual void sendMessage(const std::shared_ptr<const std::string>& ss) = 0;

    IOHandler() = default;
    virtual ~IOHandler() = default;
    IOHandler(const IOHandler&) = delete;
    IOHandler& operator=(const IOHandler&) = delete;
    IOHandler(IOHandler&&) = delete;
    IOHandler& operator=(IOHandler&&) = delete;
};
