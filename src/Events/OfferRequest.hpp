#pragma once

#include <string>

#include "InputEvent.hpp"
#include "Utils/Time.hpp"

// InputEvent representing the amount of gold offered for the current card
class OfferRequest final : public InputEvent {
   protected:
    bool isEqual(const InputEvent& obj) const final {
        auto v = static_cast<const OfferRequest&>(obj);
        return InputEvent::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    OfferRequest() : InputEvent(){};

    OfferRequest(const std::string& nickname_,
                 int gold_)
        : InputEvent(Type::OfferRequest, nickname_),
          gold(gold_){};

    OfferRequest(Timestamp time_,
                 const std::string& nickname_,
                 int gold_)
        : InputEvent(Type::OfferRequest, time_, nickname_),
          gold(gold_){};
};