#ifndef CLIENT_CONNECTED_STATE_HPP
#define CLIENT_CONNECTED_STATE_HPP

#include "state.hpp"

namespace states {
    class ConnectedState : public State {
    public:
        explicit ConnectedState(Client &client);

        ~ConnectedState() override = default;

        void create_room(const std::string &, const std::string &) override;

        void join_room(const std::string &room_name, const std::string &password) override;

        void view_rooms() override;

        void invite_messaging(const std::string &username) override;

        void accept_messaging(const std::string &username) override;

        void reject_messaging(const std::string &username) override;

        void view_users() override;

        void send_message(const std::string &message) override;

        void exit() override;
    };
}

#endif //CLIENT_CONNECTED_STATE_HPP
