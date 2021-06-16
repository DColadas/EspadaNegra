#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>

#include "Asio.hpp"
#include "Game/MatchHandler.hpp"
#include "IOHandler.hpp"
#include "MatchManager.hpp"
#include "Utils/Time.hpp"

class WebsocketSession;
class OutputEvent;
namespace Events {
    class JoinMatchRequest;
}
class WebsocketHandler : public IOHandler, public std::enable_shared_from_this<WebsocketHandler> {
   private:
    MatchHandler* currentMatch = nullptr;  //int for debug, should be Match*
    std::string nickname{};
    std::shared_ptr<MatchManager> matches = nullptr;
    WebsocketSession* ws = nullptr;

    // Variables needed for message exchange
    //using Message = std::pair<Timestamp, std::string>;
    //std::unique_ptr<std::mutex> m = std::make_unique<std::mutex>();
    //bool messageListening = false;
    //Message latestMessage;

    void joinMatch(const Events::JoinMatchRequest& req);

   public:
    WebsocketHandler(const std::shared_ptr<MatchManager>& matches);
    WebsocketHandler(const std::shared_ptr<MatchManager>& matches,
                     tcp::socket socket);
    void sendEvent(const std::shared_ptr<const OutputEvent>& event) override;
    void receiveMessage(const std::string& message) override;
    void sendMessage(const std::shared_ptr<const std::string>& ss) override;
    void setSession(WebsocketSession* session);

    void join();  //Join the matches room
    void leave();

    // Used when the session receives a new message
    void dispatchMessage(const std::string& message);
};