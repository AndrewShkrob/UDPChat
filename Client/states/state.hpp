#ifndef CLIENT_STATE_HPP
#define CLIENT_STATE_HPP

#include <string>
#include "../socket.hpp"

class Client;

namespace states {
    class State {
    public:
        explicit State(Client &client) : _client(client) {}

        virtual ~State() = default;;

        virtual void help();

        virtual void login(const std::string &username);

        virtual void connect_server(const std::string &ip, const std::string &port);

        virtual void create_room(const std::string &room_name, const std::string &password) = 0;

        virtual void join_room(const std::string &room_name, const std::string &password) = 0;

        virtual void view_rooms() = 0;

        virtual void invite_messaging(const std::string &username) = 0;

        virtual void accept_messaging(const std::string &username) = 0;

        virtual void view_users() = 0;

        virtual void send_message(const std::string &message) = 0;

        virtual void exit() = 0;

    protected:
        std::string username() const;

        Socket &socket() const;

    protected:
        Client &_client;
    };
}

#endif //CLIENT_STATE_HPP
