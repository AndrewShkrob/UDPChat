#ifndef CLIENT_EMPTY_STATE_HPP
#define CLIENT_EMPTY_STATE_HPP

#include "state.hpp"
#include "../strings.hpp"

namespace states {
    class EmptyState : State {
    public:
        explicit EmptyState(Client &client, Connection &connection, std::ostream &output)
                : State(client, connection, output) {}

        ~EmptyState() override = default;

        void login(const std::string &username) override {
            _connection.login(username);
            strings::success::username_changed(out(), username);
        }

        void connect_server(const std::string &ip, unsigned int port) override {
            if (_connection.connect(ip, port)) {
                strings::success::connected_to_server(out(), ip, port);
                _client.set_state<EmptyState>();
            } else {
                strings::error::connection_failed(out(), ip, port);
            }
        }

        void create_room(const std::string &, const std::string &) override {
            strings::error::cannot_create_room_not_connected(out());
        }

        void join_room(const std::string &room_name, const std::string &password) override {
            strings::error::cannot_join_room_not_connected(out());
        }

        virtual void view_rooms() = 0;

        virtual void invite_message(const std::string &username) = 0;

        virtual void accept_message(const std::string &username) = 0;

        virtual void view_users() = 0;

        void send_message(const std::string &message) override {
            strings::error::cannot_send_message_not_connected(out());
        }
    };
}

#endif //CLIENT_EMPTY_STATE_HPP
