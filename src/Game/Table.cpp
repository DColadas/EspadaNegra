#include "Table.hpp"

#include <algorithm>
#include <iterator>

void Table::add(Model::Card card) {
    cardsInAuction.push_back(std::move(card));
}

Model::Card Table::pop() {
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

std::vector<Model::Card> Table::getCards() const {
    std::vector<Model::Card> v;
    std::copy(cardsInAuction.begin(), cardsInAuction.end(), std::back_inserter(v));
    return v;
}
