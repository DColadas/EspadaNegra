#pragma once

#include <string>

#include "GameEvent.hpp"

// Contains necessary information to join a match
class JoinMatchRequest final : public GameEvent {
   protected:
    bool isEqual(const GameEvent& obj) const final {
        auto v = static_cast<const JoinMatchRequest&>(obj);
        return GameEvent::isEqual(obj) &&
               matchID == v.matchID &&
               nickname == v.nickname;
    }

   public:
    std::string matchID;
    std::string nickname;

    JoinMatchRequest() : GameEvent(){};

    JoinMatchRequest(const std::string& matchID_,
                     const std::string& nickname_)
        : GameEvent(Type::JoinMatchRequest),
          matchID(matchID_),
          nickname(nickname_){};

    JoinMatchRequest(Timestamp time_,
                     const std::string& matchID_,
                     const std::string& nickname_)
        : GameEvent(Type::JoinMatchRequest, time_),
          matchID(matchID_),
          nickname(nickname_){};

    /*
    JoinMatchRequest(const JoinMatchRequest& o) : GameEvent(Type::JoinMatchRequest, o.time) {
        valid = o.valid;
        matchID = o.matchID,
        nickname = o.nickname;
    };*/
    /*
    JoinMatchRequest(const JoinMatchRequest& o) : GameEvent(o) {
        matchID = o.matchID;
        nickname = o.nickname;
    }

    JoinMatchRequest& operator=(const JoinMatchRequest& o) {
        auto temp(o);
        temp.swap(*this);
        return *this;
    }

    JoinMatchRequest(JoinMatchRequest&& o) noexcept {
        }

    JoinMatchRequest& operator=(JoinMatchRequest&& o) noexcept {
        swap(o);
        return *this;
    }

    JoinMatchRequest(const JoinMatchRequest&) = default;
    JoinMatchRequest& operator=(const JoinMatchRequest&) = default;
    JoinMatchRequest(JoinMatchRequest&&) = default;
    JoinMatchRequest& operator=(JoinMatchRequest&&) = default;

    void swap(JoinMatchRequest& o) noexcept {
        using std::swap;
        GameEvent::swap(o);
        swap(matchID, o.matchID);
        swap(nickname, o.nickname);
    }

    friend void swap(JoinMatchRequest& a, JoinMatchRequest& b) noexcept {
        a.swap(b);
    }
    */
};