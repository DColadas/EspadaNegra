#pragma once

#include <string>

#include "GameEvent.hpp"
#include "PlayerResult.hpp"
#include "Time.hpp"

// PlayerResult defining which player obtained the current auctioned card
class GetCard final : public PlayerResult {
   protected:
    bool isEqual(const GameEvent& obj) const final {
        auto v = static_cast<const GetCard&>(obj);
        return PlayerResult::isEqual(obj);
    }

   public:
    GetCard() : PlayerResult(){};

    GetCard(const std::string& nickname)
        : PlayerResult(Type::GetCard, nickname){};

    GetCard(Timestamp time,
            const std::string& nickname)
        : PlayerResult(Type::GetCard, time, nickname){};
};