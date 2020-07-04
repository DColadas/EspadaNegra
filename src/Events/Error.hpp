#pragma once

#include <string>

#include "OutputEvent.hpp"

// OutputEvent sent to a player who input an invalid action
class Error final : public OutputEvent {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const Error&>(obj);
        return OutputEvent::isEqual(obj) &&
               message == v.message;
    }

   public:
    std::string message;

    Error(const std::string& message_)
        : OutputEvent(Type::Error),
          message(message_) {}
};