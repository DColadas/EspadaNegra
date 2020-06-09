#include "WebsocketSession.hpp"

#include <iostream>

WebsocketSession::WebsocketSession(tcp::socket socket)
    : ws(std::move(socket)) {
}

WebsocketSession::WebsocketSession(
    tcp::socket socket,
    const std::shared_ptr<WebsocketHandler>& handler)
    : ws(std::move(socket)), handler(handler) {
}

WebsocketSession::~WebsocketSession() {
    handler->leave();
}

void WebsocketSession::fail(error_code ec, char const* what) {
    // Errors only reported for debugging purposes
    if (ec == asio::error::operation_aborted ||
        ec == websocket::error::closed) {
        std::cerr << what << ": " << ec.message() << "\n";
        return;
    }

    std::cerr << what << ": " << ec.message() << "\n";
}

void WebsocketSession::doAccept() {
    ws.async_accept(
        [sp = shared_from_this()](
            error_code ec) {
            sp->onAccept(ec);
        });
}

void WebsocketSession::onAccept(error_code ec) {
    if (ec) {
        return fail(ec, "onAccept");
    }

    // Add this session to the list of active sessions
    handler->join();

    // Read a message
    doRead();
}

void WebsocketSession::doRead() {
    ws.async_read(
        buffer,
        [sp = shared_from_this()](
            error_code ec, std::size_t bytes) {
            sp->onRead(ec, bytes);
        });
}

void WebsocketSession::doWrite(const std::string& message) {
    std::cout << "doWrite: " << message << std::endl;
    ws.async_write(
        asio::buffer(message),
        [sp = shared_from_this()](
            error_code ec, std::size_t bytes) {
            sp->onWrite(ec, bytes);
        });
}

void WebsocketSession::onRead(error_code ec, std::size_t) {
    if (ec) {
        return fail(ec, "onRead");
    }

    std::cout << "onRead: " << beast::buffers_to_string(buffer.data()) << std::endl;
    // Notifies the handler about the new message
    handler->dispatchMessage(beast::buffers_to_string(buffer.data()));

    // Clear the buffer
    buffer.consume(buffer.size());

    // Read another message
    doRead();
}

void WebsocketSession::send(std::shared_ptr<std::string const> const& ss) {
    // Add message to queue
    queue.push_back(ss);

    // If not writing already, start to write
    if (queue.size() > 1) {
        return;
    }
    doWrite(*queue.front());
}

void WebsocketSession::onWrite(error_code ec, std::size_t) {
    if (ec) {
        return fail(ec, "onWrite");
    }

    // Front written, erase from the queue
    queue.pop_front();

    // Send the next message
    if (!queue.empty()) {
        doWrite(*queue.front());
    }
}

void WebsocketSession::run() {
    // Accept the websocket handshake
    doAccept();
}

bool WebsocketSession::isOpen() const {
    return ws.is_open();
}

void WebsocketSession::setHandler(const std::shared_ptr<WebsocketHandler>& handler) {
    this->handler = handler;
}