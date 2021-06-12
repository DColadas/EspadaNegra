#include "Server.hpp"

#include "Logging/Logger.hpp"
Server::Server(const char* address,
               unsigned short port) {
    LOG_INFO("Creating server at " + std::string(address) + ":" + std::to_string(port));
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
            LOG_TRACE("Created new session");
        });
}

void Server::run() {
    LOG_INFO("Starting server");
    listener->run();
    // If the defined signals are received, stop the io_context
    signals.async_wait(
        [this](const boost::system::error_code&, int) {
            ioc.stop();
        });
    ioc.run();
}