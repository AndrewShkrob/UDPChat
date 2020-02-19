#include "../chating_state.hpp"
#include "../connected_state.hpp"
#include "../../client.hpp"
#include "../../strings.hpp"

using namespace states;

ChattingState::ChattingState(Client &client) : State(client) {
    socket().start_chatting();
}

void ChattingState::create_room(const std::string &room_name, const std::string &password) {
//    strings::error::chatting::cannot_create_room(out());
}

void ChattingState::join_room(const std::string &room_name, const std::string &password) {
//    socket().join_room(room_name, password);
}

void ChattingState::view_rooms() {
//    out() << strings::colors::blue << strings::view_rooms << std::endl << socket().get_rooms()
//          << strings::colors::def << std::endl;
}

void ChattingState::invite_messaging(const std::string &username) {
//    socket().invite_messaging(username);
}

void ChattingState::accept_messaging(const std::string &username) {
//    socket().accept_messaging(username);
}

void ChattingState::view_users() {
//    out() << strings::colors::blue << strings::view_users << std::endl << socket().get_rooms()
//          << strings::colors::def << std::endl;
}

void ChattingState::send_message(const std::string &message) {
    // remove message from console
    std::string remove_mask("\b \b", message.size());
    out() << remove_mask;
    // send message
    socket().send_message(message);
}

void ChattingState::exit() {
    _client.out() << strings::exit_the_server << std::endl;
    socket().exit_room();
    _client.set_state<ConnectedState>();
}