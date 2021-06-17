#include "WebsocketHandler.hpp"

#include "Events/InputEvent.hpp"
#include "Events/OutputEvent.hpp"
#include "Logging/Logger.hpp"
#include "Utils/Visitor.hpp"
#include "WebsocketSession.hpp"
#include <type_traits>

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

void WebsocketHandler::sendEvent(const std::shared_ptr<const Events::OutputEvent>& event) {
    sendMessage(std::make_shared<const std::string>(Events::toMessage(*event)));
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

void WebsocketHandler::joinMatch(const Events::JoinMatchRequest& req) {
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

void WebsocketHandler::sendMessage(const std::shared_ptr<const std::string>& ss) {
    ws->send(ss);
}

void WebsocketHandler::setSession(WebsocketSession* session) {
    ws = session;
}

void WebsocketHandler::dispatchMessage(const std::string& message) {
    auto event = Events::toInputEvent(nickname, message);
    // If the event is Error, report it
    if (std::holds_alternative<Events::Error>(event)) {
        sendEvent(std::make_shared<const Events::OutputEvent>(std::get<Events::Error>(event)));
        return;
    }
    // TODO nickname check disable until a login action is implemented
    if (/*!nickname.empty() && */ !currentMatch) {
        // Not in match
        std::visit(
            visitor{
                [&](const Events::JoinMatchRequest& jmr) {
                    LOG_TRACE("JoinMatchRequest received");
                    joinMatch(jmr);
                },
                [&](const auto& /**/) {
                    LOG_ERROR("Event not implemented when not in match");
                },
            },
            event);
    } else {
        // In match
        std::visit(
            visitor{
                [&]<class T>(const T& pe) requires std::is_base_of_v<Events::PlayerEvent, T> {
                    LOG_TRACE("User " + pe.nickname + ": valid InputEvent received");
                    auto response = currentMatch->handleInputEvent(event);
                    // There is a specific message for this client (an Error)
                    if (std::holds_alternative<Events::Error>(response)) {
                        sendEvent(std::make_shared<const Events::OutputEvent>(std::move(response)));
                    }
                },
                [&](const auto& /**/) {
                    LOG_ERROR("Event not implemented when in match");
                },
            },
            event);
    }
}
