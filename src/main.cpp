#include <iostream>

#include "IO/Server.hpp"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [ADDRESS] [PORT]\n"
                  << "Example:\n\t" << argv[0] << " 127.0.0.1 8080\n";
        return EXIT_FAILURE;
    }
    auto address = argv[1];
    auto port = static_cast<unsigned short>(std::atoi(argv[2]));

    Server s(address, port);
    s.run();

    return EXIT_SUCCESS;
}
