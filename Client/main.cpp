#include <iostream>
#include "client.hpp"
#include "query.hpp"

int main() {
    Client client(std::cout);
    while (true) {
        Query q;
        std::cin >> q;
        q.process(client);
    }
    return 0;
}
