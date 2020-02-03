#ifndef CLIENT_STATE_HPP
#define CLIENT_STATE_HPP

#include <string>
#include <iostream>
#include "../client.hpp"

namespace states {
    class State {
    public:
        explicit State(Client &client, Connection &connection, std::ostream &output)
                : _client(client),
                  _connection(connection),
                  _output(output) {}

        virtual ~State() = default;;

        virtual void login(const std::string &username) = 0;

        virtual void connect_server(const std::string &ip, unsigned int port) = 0;

        virtual void create_room(const std::string &room_name, const std::string &password) = 0;

        virtual void join_room(const std::string &room_name, const std::string &password) = 0;

        virtual void view_rooms() = 0;

        virtual void invite_messaging(const std::string &username) = 0;

        virtual void accept_messaging(const std::string &username) = 0;

        virtual void view_users() = 0;

        virtual void send_message(const std::string &message) = 0;

        std::ostream &out() const {
            return _output;
        }

    protected:
        Client &_client;
        Connection &_connection;
        std::ostream &_output;
    };
}

#endif //CLIENT_STATE_HPP