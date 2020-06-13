#include "Listener.hpp"

#include <iostream>

Listener::Listener(asio::io_context& ioc,
                   tcp::endpoint endpoint,
                   std::function<void(tcp::socket&&)> onAcceptCallback_)
    : acceptor(ioc),
      socket(ioc),
      onAcceptCallback(onAcceptCallback_) {
    error_code ec;

    // Open the acceptor
    acceptor.open(endpoint.protocol(), ec);
    if (ec) {
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor.set_option(asio::socket_base::reuse_address(true));
    if (ec) {
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor.bind(endpoint, ec);
    if (ec) {
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    acceptor.listen(asio::socket_base::max_listen_connections, ec);
    if (ec) {
        fail(ec, "listen");
        return;
    }
}

void Listener::run() {
    if (!acceptor.is_open()) {
        return;
    }
    doAccept();
}

void Listener::doAccept() {
    // Start accepting a connection
    acceptor.async_accept(
        socket,
        [self = shared_from_this()](error_code ec) {
            self->onAccept(ec);
        });
}

// Report a failure
void Listener::fail(error_code ec, char const* what) {
    // Don't report canceled operations
    if (ec == asio::error::operation_aborted) {
        return;
    }
    std::cerr << what << ": " << ec.message() << "\n";
}

// Handle a connection
void Listener::onAccept(error_code ec) {
    if (ec) {
        return fail(ec, "accept");
    }
    onAcceptCallback(std::move(socket));

    // Accept another connection
    doAccept();
}
