#pragma once

#include <string>

#include "OfferRequest.hpp"
#include "PlayerResult.hpp"

// PlayerResult broadcasted when a valid OfferRequest is received
class OfferResult final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const OfferResult&>(obj);
        return PlayerResult::isEqual(obj) &&
               gold == v.gold;
    }

   public:
    int gold;

    OfferResult(const std::string& nickname_,
                int gold_)
        : PlayerResult(Type::OfferResult, nickname_),
          gold(gold_) {}

    // Does not check if the OfferRequest is valid
    explicit OfferResult(const OfferRequest& o)
        : PlayerResult(Type::OfferResult, o.nickname),
          gold(o.gold) {}
};