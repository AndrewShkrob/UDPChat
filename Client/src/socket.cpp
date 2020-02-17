#include "../socket.hpp"
#include <iostream>

using boost::thread;
using boost::asio::ip::udp;

Socket::Socket(std::ostream &os) : _out(os), _socket(_io_service) {}

Socket::~Socket() {
    _thread->join();
}

void Socket::connect(const std::string &ip, const std::string &port) {
    udp::resolver resolver(_io_service);
    udp::resolver::query query(udp::v4(), ip, port);
    udp::resolver::iterator endpoint = resolver.resolve(query);
    _receiver_endpoint = *endpoint;
    boost::asio::connect(_socket, endpoint);
    handle_connect();
    start_receive();
    _thread = std::make_unique<boost::thread>(boost::bind(&boost::asio::io_service::run, &_io_service));
}

bool Socket::disconnect() {
    _thread->interrupt();
    return true;
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
    ChatMessage msg;
    msg.WriteToMessage(message);
    _socket.async_send_to(boost::asio::buffer(msg.GetRawData()), _receiver_endpoint,
                          boost::bind(&Socket::handle_send, this, msg,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
    return true;
}

void Socket::set_username(std::string username) {
    _username = std::move(username);
}

void Socket::handle_connect() {
    ChatMessage initial{std::time(nullptr)};
    _socket.send_to(boost::asio::buffer(initial.GetRawData()), _receiver_endpoint);
    _socket.receive_from(boost::asio::buffer(initial.GetRawData()), _receiver_endpoint);
}

void Socket::handle_send(ChatMessage &msg,
                         const boost::system::error_code & /*error*/,
                         std::size_t /*bytes_transferred*/) {
    //std::cout << "Message: " + *message << '\n';
}

void Socket::start_receive() {
    _socket.async_receive_from(boost::asio::buffer(_chat_message.GetRawData()), _temp_endpoint,
                               boost::bind(&Socket::handle_receive,
                                           this, boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
}

void Socket::handle_receive(const boost::system::error_code &error,
                            std::size_t /*bytes_transferred*/) {
    if (!error || error == boost::asio::error::message_size) {
        _chat_message.Decode();
        _out << _chat_message.ReadText() << '\n';
    }
    start_receive();
}