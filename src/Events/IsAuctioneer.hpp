#pragma once

#include <string>

#include "PlayerResult.hpp"

// PlayerResult informing about which is the current auctioneer
class IsAuctioneer final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const IsAuctioneer&>(obj);
        return PlayerResult::isEqual(obj);
    }

   public:
    IsAuctioneer(const std::string& nickname_)
        : PlayerResult(Type::IsAuctioneer, nickname_) {}
};