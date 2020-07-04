#pragma once

#include <string>

#include "InputEvent.hpp"
#include "Utils/Time.hpp"

// InputEvent by which the player is not interested in the current attack/auction phase
class PassRequest final : public InputEvent {
   protected:
    bool isEqual(const InputEvent& obj) const final {
        auto v = static_cast<const PassRequest&>(obj);
        return InputEvent::isEqual(obj);
    }

   public:
    PassRequest() : InputEvent(){};

    PassRequest(const std::string& nickname_)
        : InputEvent(Type::PassRequest, nickname_){};

    PassRequest(Timestamp time_,
                const std::string& nickname_)
        : InputEvent(Type::PassRequest, time_, nickname_){};
};