#pragma once

#include <string>

#include "PlayerResult.hpp"

// PlayerResult defining which player obtained the current auctioned card
class GetCard final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const GetCard&>(obj);
        return PlayerResult::isEqual(obj);
    }

   public:
    GetCard(const std::string& nickname_)
        : PlayerResult(Type::GetCard, nickname_) {}
};