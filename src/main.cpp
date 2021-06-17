#include <map>
#include <numeric>
#include <vector>

#include "Model/Card.hpp"
#include "Model/CardModel.hpp"
#include "Model/Deck.hpp"
#include "IO/Server.hpp"
#include "Logging/Logger.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        LOG_ERROR(
            "Usage: server [ADDRESS] [PORT]\n"
            "Example:\n\tserver 127.0.0.1 8080\n");
        return EXIT_FAILURE;
    }

    // Set Logger severity
    Logger::get().setLevel(Logger::LogLevel::Trace);

    // Initialise the CardModel database
    // TODO move this to an actual database or file
    Model::CardModel::add(1, "Inquisidor", 1, 0, 1, false);
    Model::CardModel::add(2, "Campo", 2, 0, 1, false);
    Model::CardModel::add(3, "Guerrero", 2, 1, 0, false);
    Model::CardModel::add(4, "Caballero", 3, 0, 0, false);
    Model::CardModel::add(5, "Ciudad", 0, 4, 0, false);
    Model::CardModel::add(6, "Campesino", 0, 1, 2, false);
    Model::CardModel::add(7, "Rey", 0, 0, 5, false);

    // Initialise Card database
    std::map<int, int> cards{
        //modelId, amount
        {1, 10},
        {2, 9},
        {3, 3},
        {4, 1},
        {5, 2},
        {6, 15},
        {7, 8},
    };
    int cardId = 1;
    for (const auto& c : cards) {
        for (int i = 0; i < c.second; ++i) {
            Model::Card::add(cardId, {}, c.first);
            ++cardId;
        }
    }

    // Initialise Deck database
    std::vector<int> deck1(48);  // 48 is the amount of cards
    std::iota(deck1.begin(), deck1.end(), 1);
    Model::Deck::add(1, std::move(deck1));

    auto address = argv[1];
    auto port = static_cast<unsigned short>(std::atoi(argv[2]));

    Server s(address, port);
    s.run();

    return EXIT_SUCCESS;
}
