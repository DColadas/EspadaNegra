#pragma once

#include <deque>
#include <memory>
#include <string>

#include "Asio.hpp"
#include "Beast.hpp"
#include "IOHandler.hpp"

class MatchManager;

class WebsocketSession : public IOHandler, public std::enable_shared_from_this<WebsocketSession> {
   public:
    WebsocketSession(const std::shared_ptr<MatchManager>& manager,
                     tcp::socket socket);
    ~WebsocketSession();

    // Start the connection
    void run() override;

    // Send a message to the client
    void sendMessage(const std::shared_ptr<const std::string>& message) override;

    // True if the connection is valid
    [[nodiscard]] bool isOpen() const override;

   private:
    websocket::stream<tcp::socket> ws;
    beast::flat_buffer buffer;
    std::deque<std::shared_ptr<const std::string>> queue;  // Message queue

    void fail(error_code ec, char const* what);
    void doAccept();
    void doRead();
    void doWrite(const std::string& message);
    void onAccept(error_code ec);
    void onRead(error_code ec, std::size_t bytes_transferred);
    void onWrite(error_code ec, std::size_t bytes_transferred);
};
