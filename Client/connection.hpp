#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <string>

class Connection {
public:

    void login(const std::string &username) {
        _username = username;
    }

    bool connect(const std::string &ip, unsigned int port) {
        _ip = ip;
        _port = port;
        return true;
    }

private:
    std::string _username;
    std::string _ip;
    unsigned int _port;
};

#endif //CLIENT_CONNECTION_HPP
