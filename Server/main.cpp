#include <iostream>
#include "server.hpp"

int main(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: chat_server <port>\n";
            return 1;
        }
        Server server(std::atoi(argv[1]));
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}