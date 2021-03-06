#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "Asio.hpp"
#include "Manager/MatchManager.hpp"
#include "Manager/ServerManager.hpp"

class Listener;

// Accepts incoming connections and launches the sessions
class Server {
   private:
    asio::io_context ioc;
    std::shared_ptr<Listener> listener;
    // Works until the defined signals are received
    asio::signal_set signals = asio::signal_set(ioc, SIGINT, SIGTERM);
    std::shared_ptr<MatchManager> matches = std::make_shared<ServerManager>();

   public:
    Server(const char* address, unsigned short port);

    // Start accepting incoming connections
    void run();
};
