#pragma once

#include "OutputEvent.hpp"

class Card;
// OutputEvent that informs about a new card in the auction
class Draw final : public OutputEvent {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const Draw&>(obj);
        return OutputEvent::isEqual(obj) &&
               card == v.card;
    }

   public:
    const Card& card;

    Draw(const Card& card_)
        : OutputEvent(Type::Draw),
          card(card_) {}
};