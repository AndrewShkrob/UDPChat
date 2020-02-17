#include "../client.hpp"
#include "../strings.hpp"

Client::Client(std::ostream &out) : _out(out) {
    set_state<states::EmptyState>();
}

std::ostream &Client::out() const {
    return _out;
}

void Client::help() const {
    _current_state->help();
}

void Client::login(const std::string &username) {
    _current_state->login(username);
}

void Client::connect(const std::string &address, const std::string &port) {
    _current_state->connect_server(address, port);
}

void Client::create_room(const std::string &room_name, const std::string &password) {
    _current_state->create_room(room_name, password);
}

void Client::join_room(const std::string &room_name, const std::string &password) {
    _current_state->join_room(room_name, password);
}

void Client::view_rooms() const {
    _current_state->view_rooms();
}

void Client::invite_messaging(const std::string &username) const {
    _current_state->invite_messaging(username);
}

void Client::accept_messaging(const std::string &username) {
    _current_state->accept_messaging(username);
}

void Client::view_users() const {
    _current_state->view_users();
}

void Client::exit() {
    _current_state->exit();
}

void Client::send_message(const std::string &message) const {
    _current_state->send_message(message);
}

void Client::unknown_command() const {
    _out << strings::unknown_command << std::endl;
    _out << strings::help_info;
}