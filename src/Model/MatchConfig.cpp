#include "MatchConfig.hpp"

namespace Model {

MatchConfig::MatchConfig(unsigned int numPlayers_)
    : numPlayers(numPlayers_) {
    //TODO
}

//void MatchConfig::applyModifier(const GameModifier& modifier) {
//    //TODO
//}

bool MatchConfig::operator==(const MatchConfig& rhs) const {
    // Compare every attribute
    return numPlayers == rhs.numPlayers &&
           cardsPerTurn == rhs.cardsPerTurn &&
           initialGold == rhs.initialGold &&
           auctioneerGoldDisadvantage == rhs.auctioneerGoldDisadvantage;
}

bool MatchConfig::operator!=(const MatchConfig& rhs) const {
    return !(*this == rhs);
}

void to_json(nlohmann::json& j, const MatchConfig& config) {
    j = {{"numPlayers", config.numPlayers}};
}

}  // namespace Model