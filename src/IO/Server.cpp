#include "Server.hpp"

#include "IOHandler.hpp"
#include "Listener.hpp"
#include "Logging/Logger.hpp"
#include "Manager/MatchManager.hpp"
#include "WebsocketSession.hpp"

Server::Server(const char* address,
               unsigned short port) {
    LOG_INFO("Creating server at " + std::string(address) + ":" + std::to_string(port));
    listener = std::make_shared<Listener>(
        ioc,
        tcp::endpoint{asio::ip::make_address(address), port},
        [this](tcp::socket&& socket) {
            std::shared_ptr<IOHandler> io = std::make_shared<WebsocketSession>(matches, std::move(socket));
            io->run();
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