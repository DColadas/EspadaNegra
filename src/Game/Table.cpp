#include "Table.hpp"

void Table::add(Card card) {
    cardsInAuction.push_back(std::move(card));
}

Card Table::pop() {
    const auto card = cardsInAuction.front();
    cardsInAuction.pop_front();
    return card;
}

bool Table::isEmpty() const {
    return cardsInAuction.empty();
}

void Table::discard() {
    discarded.push_back(cardsInAuction.front());
    cardsInAuction.pop_front();
}