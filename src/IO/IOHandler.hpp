#pragma once

#include <memory>
#include <string>

class MatchManager;

class IOHandler {
   public:
    IOHandler(const std::shared_ptr<MatchManager>& manager);

    // Join or leave the MatchManager
    void join();
    void leave();

    // Start the connection
    virtual void run() = 0;

    // Send message to the client
    virtual void sendMessage(const std::shared_ptr<const std::string>& ss) = 0;

    // True if the connection is valid
    [[nodiscard]] virtual bool isOpen() const = 0;

    virtual ~IOHandler() = default;
    IOHandler(const IOHandler&) = delete;
    IOHandler& operator=(const IOHandler&) = delete;
    IOHandler(IOHandler&&) = delete;
    IOHandler& operator=(IOHandler&&) = delete;

   protected:
    std::shared_ptr<MatchManager> manager;
};
