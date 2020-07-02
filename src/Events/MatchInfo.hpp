#pragma once

#include <string>
#include <vector>

#include "OutputEvent.hpp"

class MatchConfig;
class Player;
class Deck;
// OutputEvent sent to every player who just joined the match
class MatchInfo final : public OutputEvent {
   protected:
    bool isEqual(const OutputEvent& obj) const final {
        auto v = static_cast<const MatchInfo&>(obj);
        return OutputEvent::isEqual(obj) &&
               config == v.config &&
               players == v.players &&
               deck == v.deck;
    }

   public:
    const MatchConfig& config;
    const std::vector<Player>& players;
    const Deck& deck;

    MatchInfo(const MatchConfig& config_,
              const std::vector<Player>& players_,
              const Deck& deck_)
        : OutputEvent(Type::MatchInfo),
          config(config_),
          players(players_),
          deck(deck_) {}
};