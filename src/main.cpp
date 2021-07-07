#include <map>
#include <numeric>
#include <vector>

#include "Model/Card.hpp"
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

    // Initialise Card database
    Model::Card::init();

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
