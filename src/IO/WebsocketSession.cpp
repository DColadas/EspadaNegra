#include "WebsocketSession.hpp"

#include "Logging/Logger.hpp"
#include "Manager/MatchManager.hpp"

WebsocketSession::WebsocketSession(const std::shared_ptr<MatchManager>& manager_,
                                   tcp::socket socket)
    : IOHandler(manager_), ws(std::move(socket)) {}

WebsocketSession::~WebsocketSession() {
    leave();
}

void WebsocketSession::fail(error_code ec, char const* what) {
    // Errors only reported for debugging purposes
    if (ec == asio::error::operation_aborted ||
        ec == websocket::error::closed) {
        LOG_TRACE(what + std::string(": ") + ec.message());
        return;
    }
    LOG_ERROR(what + std::string(": ") + ec.message());
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
    join();

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
    //LOG_TRACE("doWrite: " + message);
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

    //LOG_TRACE("onRead: " + beast::buffers_to_string(buffer.data()));
    // Notifies the handler about the new message
    manager->dispatchMessage(*this, beast::buffers_to_string(buffer.data()));

    // Clear the buffer
    buffer.consume(buffer.size());

    // Read another message
    doRead();
}

void WebsocketSession::sendMessage(const std::shared_ptr<const std::string>& message) {
    // Add message to queue
    queue.push_back(message);

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
