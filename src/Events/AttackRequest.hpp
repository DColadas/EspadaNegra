#pragma once

#include <string>

#include "InputEvent.hpp"
#include "Utils/Time.hpp"

// InputEvent by which the player intents to attack the current card
class AttackRequest final : public InputEvent {
   protected:
    bool isEqual(const InputEvent& obj) const final {
        auto v = static_cast<const AttackRequest&>(obj);
        return InputEvent::isEqual(obj);
    }

   public:
    AttackRequest() : InputEvent(){};

    AttackRequest(const std::string& nickname_)
        : InputEvent(Type::AttackRequest, nickname_){};

    AttackRequest(Timestamp time_,
                  const std::string& nickname_)
        : InputEvent(Type::AttackRequest, time_, nickname_){};
};