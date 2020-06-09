#include "WebsocketHandler.hpp"

#include <iostream>

#include "Events/JSONParser.hpp"
#include "WebsocketSession.hpp"

WebsocketHandler::WebsocketHandler(const std::shared_ptr<MatchManager>& matches)
    : matches(matches) {
}

WebsocketHandler::WebsocketHandler(const std::shared_ptr<MatchManager>& matches,
                                   tcp::socket socket)
    : matches(matches) {
    //TODO cannot do this
    auto s = shared_from_this();

    auto wsShared = std::make_shared<WebsocketSession>(
        std::move(socket),
        s);
    ws = wsShared.get();
    ws->run();
}

void WebsocketHandler::sendEvent(const std::shared_ptr<const GameEvent>& event) {
}

void WebsocketHandler::join() {
    matches->join(*this);
}

void WebsocketHandler::leave() {
    matches->leave(*this);
}

void WebsocketHandler::joinMatch(const JoinMatchRequest& req) {
    if (inMatch) {
        std::cout << "Already in match!" << std::endl;
        return;
    }
    currentMatch = matches->joinMatch(req);
    inMatch = currentMatch ? true : false;
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
    auto event = JSONParser::messageToGameEvent(now, message);
    switch (event->type) {
        case GE::JoinMatchRequest:
        //TODO try static_cast (known it won't fail)
            joinMatch(*dynamic_cast<JoinMatchRequest*>(event.get()));
            break;
        case GE::PlayerEvent:
            //TODO Do something
            break;
        default:
            break;
    }
}
