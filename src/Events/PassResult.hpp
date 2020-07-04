#pragma once

#include <string>

#include "PassRequest.hpp"
#include "PlayerResult.hpp"

// PlayerResult broadcasted when a valid PassRequest is received
class PassResult final : public PlayerResult {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const PassResult&>(obj);
        return PlayerResult::isEqual(obj);
    }

   public:
    PassResult(const std::string& nickname_)
        : PlayerResult(Type::PassResult, nickname_) {}

    // Does not check if the PassRequest is valid
    explicit PassResult(const PassRequest& o)
        : PlayerResult(Type::PassResult, o.nickname) {}
};