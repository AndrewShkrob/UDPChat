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
            strings::error::not_connected::cannot_create_room(out());
        }

        void join_room(const std::string &room_name, const std::string &password) override {
            strings::error::not_connected::cannot_join_room(out());
        }

        virtual void view_rooms() override {
            strings::error::not_connected::cannot_view_rooms(out());
        }

        virtual void invite_messaging(const std::string &username) override {
            strings::error::not_connected::cannot_invite_messaging(out());
        }

        virtual void accept_messaging(const std::string &username) override {
            strings::error::not_connected::cannot_accept_messaging(out());
        }

        virtual void view_users() override {
            strings::error::not_connected::cannot_view_users(out());
        }

        void send_message(const std::string &message) override {
            strings::error::not_connected::cannot_send_message(out());
        }
    };
}

#endif //CLIENT_EMPTY_STATE_HPP
