#pragma once

#include <cstdlib>
#include <deque>
#include <memory>
#include <string>

#include "Asio.hpp"
#include "Beast.hpp"
#include "WebsocketHandler.hpp"

//TODO implement BaseWebsocket using template method to define what to do when a message is received
class WebsocketSession : public std::enable_shared_from_this<WebsocketSession> {
    websocket::stream<tcp::socket> ws;
    beast::flat_buffer buffer;
    std::shared_ptr<WebsocketHandler> handler;
    std::deque<std::shared_ptr<const std::string>> queue;

    void fail(error_code ec, char const* what);
    void doAccept();
    void doRead();
    void doWrite(const std::string& message);
    void onAccept(error_code ec);
    void onRead(error_code ec, std::size_t bytes_transferred);
    void onWrite(error_code ec, std::size_t bytes_transferred);

   public:
    WebsocketSession(tcp::socket socket);
    WebsocketSession(
        tcp::socket socket,
        const std::shared_ptr<WebsocketHandler>& handler);

    ~WebsocketSession();

    void run();

    void setHandler(const std::shared_ptr<WebsocketHandler>& handler);

    // True if the connection is valid
    bool isOpen() const;

    // Send a message
    void send(const std::shared_ptr<const std::string>& ss);
};
