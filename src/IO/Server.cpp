#include "Server.hpp"

#include <iostream>
Server::Server(const char* address,
               unsigned short port) {
    listener = std::make_shared<Listener>(
        ioc,
        tcp::endpoint{asio::ip::make_address(address), port},
        [this](tcp::socket&& socket) {
            //TODO make this construction in a factory
            auto wsHandler = std::make_shared<WebsocketHandler>(matches);
            auto wsSession = std::make_shared<WebsocketSession>(std::move(socket));
            wsHandler->setSession(wsSession.get());
            wsSession->setHandler(wsHandler);
            wsSession->run();
            std::cout << "Created new session" << std::endl;
        });
}

void Server::run() {
    listener->run();
    // If the defined signals are received, stop the io_context
    signals.async_wait(
        [this](const boost::system::error_code&, int) {
            ioc.stop();
        });
    ioc.run();
}