#include "../empty_state.hpp"
#include "../../client.hpp"
#include "../../strings.hpp"

using namespace states;

EmptyState::EmptyState(Client &client) : State(client) {}

void EmptyState::create_room(const std::string &, const std::string &) {
    strings::error::not_connected::cannot_create_room(_client.out());
}

void EmptyState::join_room(const std::string &room_name, const std::string &password) {
    strings::error::not_connected::cannot_join_room(_client.out());
}

void EmptyState::view_rooms() {
    strings::error::not_connected::cannot_view_rooms(_client.out());
}

void EmptyState::invite_messaging(const std::string &username) {
    strings::error::not_connected::cannot_invite_messaging(_client.out());
}

void EmptyState::accept_messaging(const std::string &username) {
    strings::error::not_connected::cannot_accept_messaging(_client.out());
}

void EmptyState::view_users() {
    strings::error::not_connected::cannot_view_users(_client.out());
}

void EmptyState::send_message(const std::string &message) {
    strings::error::not_connected::cannot_send_message(_client.out());
}

void EmptyState::exit() {
    _client.out() << strings::exit_the_chat << std::endl;
    ::exit(0);
}