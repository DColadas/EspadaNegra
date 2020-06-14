#include "Player.hpp"

#include <iterator>
#include <numeric>

Player::Player(const std::string& name_)
    : name(std::move(name_)) {
}

int Player::getTotalAttack() const {
    return std::accumulate(activeCards.begin(), activeCards.end(), 0,
                           [](int a, const Card b) {
                               return a + b.getAttack();
                           });
}

int Player::getTotalProduction() const {
    const auto active = std::accumulate(activeCards.begin(),
                                        activeCards.end(),
                                        0, [](int a, const Card b) {
                                            return a + b.getProduction();
                                        });
    const auto recent = std::accumulate(purchasedThisTurnCards.begin(),
                                        purchasedThisTurnCards.end(),
                                        0, [](int a, const Card b) {
                                            return a + b.getProduction();
                                        });
    return active + recent;
}

int Player::getTotalVictory() const {
    const auto active = std::accumulate(activeCards.begin(),
                                        activeCards.end(),
                                        0, [](int a, const Card b) {
                                            return a + b.getVictory();
                                        });
    const auto recent = std::accumulate(purchasedThisTurnCards.begin(),
                                        purchasedThisTurnCards.end(),
                                        0, [](int a, const Card b) {
                                            return a + b.getVictory();
                                        });
    return active + recent;
}

void Player::onTurnEnd() {
    // Move purchased cards to active
    activeCards.insert(activeCards.end(),
                       std::make_move_iterator(purchasedThisTurnCards.begin()),
                       std::make_move_iterator(purchasedThisTurnCards.end()));
    purchasedThisTurnCards.clear();

    // Reset attack flag
    hasAttacked = false;

    // Produce (should be carried out by Match)
    //earn(getTotalProduction());
}

void Player::addCard(const Card& card) {
    purchasedThisTurnCards.push_back(std::move(card));
}

int Player::attack() {
    hasAttacked = true;
    return getTotalAttack();
}

bool Player::canOffer(int amount) const {
    return amount <= gold;
}

bool Player::canAttack() const {
    return !hasAttacked && getTotalAttack() > 0;
}

bool Player::canAttack(int amount) const {
    return !hasAttacked && getTotalAttack() > amount;
}

void Player::pay(int amount) {
    //TODO add panic if amount > gold
    gold -= amount;
}

void Player::earn(int amount) {
    gold += amount;
}
