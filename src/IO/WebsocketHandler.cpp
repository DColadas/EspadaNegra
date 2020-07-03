#include "WebsocketHandler.hpp"

#include "Events/InputEvent.hpp"
#include "Events/JoinMatchRequest.hpp"
#include "Logging/Logger.hpp"
#include "Parsing/JSONParser.hpp"
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

void WebsocketHandler::sendEvent(const std::shared_ptr<const OutputEvent>& /*event*/) {
}

void WebsocketHandler::join() {
    matches->join(*this);
}

void WebsocketHandler::leave() {
    if (currentMatch) {
        currentMatch->removePlayer(nickname);
    }
    matches->leave(*this);
}

void WebsocketHandler::joinMatch(const JoinMatchRequest& req) {
    if (currentMatch) {
        LOG_DEBUG(nickname + " already was in match");
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
    using ET = InputEvent::Type;
    const auto now = std::chrono::system_clock::now();
    auto event = JSONParser::messageToInputEvent(now, nickname, message);
    // TODO nickname check disable until a login action is implemented
    if (/*!nickname.empty() && */ !currentMatch) {
        // Not in match
        switch (event->getType()) {
            case ET::JoinMatchRequest:
                LOG_TRACE("JoinMatchRequest received");
                joinMatch(*static_cast<JoinMatchRequest*>(event.get()));
                break;
            default:
                LOG_ERROR("Event not implemented when not in match");
        }
    } else {
        // In match
        switch (event->getType()) {
            case ET::AttackRequest:
            case ET::PassRequest:
            case ET::OfferRequest:
                LOG_TRACE("User " + nickname + ": valid InputEvent received");
                currentMatch->handleInputEvent(event.get());
                break;
            default:
                LOG_ERROR("Event not implemented when in match");
        }
    }
}
