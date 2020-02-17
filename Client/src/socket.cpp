#include "../socket.hpp"

using boost::thread;

Socket::Socket() : _socket(_io_service) {
    _thread = thread(boost::bind(&boost::asio::io_service::run, &_io_service));
    _thread.join();
}

bool Socket::connect(const std::string &ip, const std::string &port) {
    return true;
}

bool Socket::disconnect() {
    _thread.interrupt();
}

bool Socket::create_room(const std::string &room_name, const std::string &password) {
    return true;
}

bool Socket::join_room(const std::string &room_name, const std::string &password) {
    return true;
}

std::string Socket::get_rooms() {
    return "abacaba (locked)\nasd2345";
}

bool Socket::invite_messaging(const std::string &username) {
    return true;
}

bool Socket::accept_messaging(const std::string &username) {
    return true;
}

std::string Socket::get_users() {
    return "user1\nuser2\nuser3";
}

bool Socket::send_message(const std::string &message) {
    return true;
}