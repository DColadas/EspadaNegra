#pragma once

#include <functional>
#include <memory>
#include <string>

#include "Asio.hpp"

// Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener> {
    tcp::acceptor acceptor;
    tcp::socket socket;
    std::function<void(tcp::socket&&)> onAcceptCallback;

    void fail(error_code ec, char const* what);
    void doAccept();
    void onAccept(error_code ec);

   public:
    Listener(asio::io_context& ioc,
             tcp::endpoint endpoint,
             std::function<void(tcp::socket&&)> onAcceptCallback);

    // Start accepting incoming connections
    void run();
};
