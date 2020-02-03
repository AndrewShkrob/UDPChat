#ifndef CLIENT_CLIENT_HPP
#define CLIENT_CLIENT_HPP

#include <string>
#include <iostream>
#include "states/state.hpp"
#include "states/empty_state.hpp"
#include "connection.hpp"

class Client {
public:
    explicit Client(std::ostream &out) : _out(out) {
        set_state<states::EmptyState>();
    }

    void login(const std::string &username) {
        _current_state.login(username);
    }

    void connect(const std::string &ip, unsigned int port) {
        _current_state.connect_server(ip, port);
    }

    template<class State>
    void set_state() {
        _current_state = State(this, _connection, _out);
    }

private:
    std::ostream &_out;
    states::State _current_state;
    Connection _connection;
};

#endif //CLIENT_CLIENT_HPP
