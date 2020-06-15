#include "WebsocketHandler.hpp"

#include <iostream>

#include "Events/JSONParser.hpp"
#include "WebsocketSession.hpp"

WebsocketHandler::WebsocketHandler(const std::shared_ptr<MatchManager>& matches_)
    : matches(matches_) {
}

WebsocketHandler::WebsocketHandler(const std::shared_ptr<MatchManager>& matches_,
                                   tcp::socket socket)
    : matches(matches_) {
    //TODO cannot do this
    auto s = shared_from_this();

    auto wsShared = std::make_shared<WebsocketSession>(
        std::move(socket),
        s);
    ws = wsShared.get();
    ws->run();
}

void WebsocketHandler::sendEvent(const std::shared_ptr<const GameEvent>& /*event*/) {
}

void WebsocketHandler::join() {
    matches->join(*this);
}

void WebsocketHandler::leave() {
    matches->leave(*this);
}

void WebsocketHandler::joinMatch(const JoinMatchRequest& req) {
    if (currentMatch) {
        std::cout << "Already in match!" << std::endl;
        return;
    }
    currentMatch = matches->joinMatch(*this, req);
    if (currentMatch) {
        // Accepted into the match
        nickname = req.nickname;
    }
}

void WebsocketHandler::receiveMessage(const std::string& message) {
    matches->send(message);
}

void WebsocketHandler::sendMessage(std::shared_ptr<std::string const> const& ss) {
    ws->send(ss);
}

void WebsocketHandler::setSession(WebsocketSession* session) {
    ws = session;
}

void WebsocketHandler::dispatchMessage(const std::string& message) {
    using GE = GameEvent::Type;
    const auto now = std::chrono::system_clock::now();
    auto event = JSONParser::messageToGameEvent(now, nickname, message);
    // TODO nickname check disable until a login action is implemented
    if (/*!nickname.empty() && */ !currentMatch) {
        // Not in match
        switch (event->getType()) {
            case GE::JoinMatchRequest:
                joinMatch(*static_cast<JoinMatchRequest*>(event.get()));
                break;
            default:
                std::cerr << "Event not implemented, not in match" << std::endl;
        }
    } else {
        // In match
        switch (event->getType()) {
            case GE::Attack:
            case GE::Pass:
            case GE::Offer:
                //TODO Do something
                std::cout << "Event of type " << static_cast<int>(event->getType()) << " received." << std::endl;
                break;
            default:
                std::cerr << "Event not implemented, in match" << std::endl;
                break;
        }
    }
}
