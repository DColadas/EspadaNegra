#include "Player.hpp"

#include <iterator>
#include <numeric>

#include "Logging/Logger.hpp"

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

    // Reset attack flag and pass flag
    hasAttacked = false;
    hasPassed = false;

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

void Player::pass() {
    hasPassed = true;
}

bool Player::canOffer(int amount) const {
    return !hasPassed && amount <= gold;
}

bool Player::canAttack(int amount) const {
    return !hasAttacked && !hasPassed && getTotalAttack() > amount;
}

bool Player::canAttack() const {
    return canAttack(0);
}

void Player::pay(int amount) {
    LOG_PANIC_IF(!canOffer(amount), name + " cannot offer " + std::to_string(amount) + " gold");
    gold -= amount;
}

void Player::earn(int amount) {
    gold += amount;
}
