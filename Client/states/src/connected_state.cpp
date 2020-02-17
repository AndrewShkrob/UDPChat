#include "../connected_state.hpp"
#include "../../client.hpp"
#include "../../strings.hpp"

using namespace states;

ConnectedState::ConnectedState(Client &client) : State(client) {}

void ConnectedState::create_room(const std::string &room_name, const std::string &password) {
    if (socket().create_room(room_name, password)) {
        strings::success::room_created(_client.out(), room_name);
    } else {
        strings::error::room_creation_failed(_client.out(), room_name);
    }
}

void ConnectedState::join_room(const std::string &room_name, const std::string &password) {
    strings::error::not_connected::cannot_join_room(_client.out());
}

void ConnectedState::view_rooms() {
    _client.out() << socket().get_rooms() << std::endl;
}

void ConnectedState::invite_messaging(const std::string &username) {
    strings::error::not_connected::cannot_invite_messaging(_client.out());
}

void ConnectedState::accept_messaging(const std::string &username) {
    strings::error::not_connected::cannot_accept_messaging(_client.out());
}

void ConnectedState::view_users() {
    strings::error::not_connected::cannot_view_users(_client.out());
}

void ConnectedState::send_message(const std::string &message) {
    strings::error::not_connected::cannot_send_message(_client.out());
}

void ConnectedState::exit() {
    _client.out() << strings::exit_the_chat << std::endl;
    socket().disconnect();
}