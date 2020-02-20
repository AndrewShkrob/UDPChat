#include "../connected_state.hpp"
#include "../../client.hpp"
#include "../../strings.hpp"
#include "../chating_state.hpp"

using namespace states;

ConnectedState::ConnectedState(Client &client) : State(client) {}

void ConnectedState::create_room(const std::string &room_name, const std::string &password) {
    const auto &[result, message] = socket().create_room(room_name, password);
    if (result) {
        strings::success::room_created(_client.out(), room_name);
    } else {
        strings::error::room_creation_failed(_client.out(), room_name, message);
    }
}

void ConnectedState::join_room(const std::string &room_name, const std::string &password) {
    const auto &[result, message] = socket().join_room(room_name, password);
    if (result) {
        strings::success::joined_room(_client.out(), room_name);
        _client.set_state<ChattingState>();
    } else {
        strings::error::room_joining_failed(_client.out(), room_name, message);
    }
}

void ConnectedState::view_rooms() {
    out() << strings::colors::blue << strings::view_rooms << std::endl << socket().get_rooms()
          << strings::colors::def << std::endl;
}

void ConnectedState::invite_messaging(const std::string &username) {
    out() << strings::colors::cyan << "Sending invitation and waiting for the answer..." << strings::colors::def
          << std::endl;
    bool result = socket().invite_messaging(username);
    if (result) {
        out() << strings::colors::cyan << "You are connected to private room" << strings::colors::def << std::endl;
        _client.set_state<ChattingState>();
    } else {
        out() << strings::colors::red << "Bad request" << strings::colors::def << std::endl;
    }
}

void ConnectedState::accept_messaging(const std::string &username) {
    out() << strings::colors::cyan << "Accepting invitation..." << strings::colors::def
          << std::endl;
    bool result = socket().accept_messaging(username);
    if (result) {
        out() << strings::colors::cyan << "You are connected to private room" << strings::colors::def << std::endl;
        _client.set_state<ChattingState>();
    } else {
        out() << strings::colors::red << "Bad request" << strings::colors::def << std::endl;
    }
}

void ConnectedState::reject_messaging(const std::string &username) {
    out() << strings::colors::cyan << "Rejecting invitation..." << strings::colors::def
          << std::endl;
    bool result = socket().reject_messaging(username);
    if (result) {
        out() << strings::colors::green << "Rejected" << strings::colors::def << std::endl;
    } else {
        out() << strings::colors::red << "Bad request" << strings::colors::def << std::endl;
    }
}

void ConnectedState::view_users() {
    out() << strings::colors::blue << strings::view_users << std::endl << socket().get_users()
          << strings::colors::def << std::endl;
}

void ConnectedState::send_message(const std::string &message) {
    strings::error::not_chatting::cannot_send_message(out());
}

void ConnectedState::exit() {
    _client.out() << strings::exit_the_server << std::endl;
    socket().disconnect();
    _client.set_state<EmptyState>();
}