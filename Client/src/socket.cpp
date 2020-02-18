#include "../socket.hpp"

using boost::thread;
using boost::asio::ip::udp;

Socket::Socket(std::ostream &os) : _out(os), _socket(_io_service) {}

Socket::~Socket() {
    _thread->join();
}

std::pair<bool, std::string> Socket::connect(const std::string &ip, const std::string &port) {
    udp::resolver resolver(_io_service);
    udp::resolver::query query(udp::v4(), ip, port);
    udp::resolver::iterator endpoint = resolver.resolve(query);
    _receiver_endpoint = *endpoint;
    boost::asio::connect(_socket, endpoint);
    const auto &[result, message] = handle_connect();
    if (!result)
        return {result, message};
//    start_receive();
//    _thread = std::make_unique<boost::thread>(boost::bind(&boost::asio::io_service::run, &_io_service));
    return {true, ""};
}

bool Socket::disconnect() {
    try {
        _thread->detach();
        _socket.close();
    } catch (...) {}
    return true;
}

std::pair<bool, std::string> Socket::create_room(const std::string &room_name, const std::string &password) {
    ChatMessage query("/create_room " + room_name + " " + password);
    _socket.send_to(boost::asio::buffer(query.get_raw_data()), _receiver_endpoint);
    _socket.receive_from(boost::asio::buffer(query.get_raw_data()), _receiver_endpoint);
    query.decode();
    std::string data = query.get_text();
    if (query.get_type() == MessageType::UNKNOWN)
        return {false, ""};
    if (data != "Ok")
        return {false, data};
    else
        return {true, ""};
}

bool Socket::join_room(const std::string &room_name, const std::string &password) {
    return true;
}

bool Socket::exit_room() {
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
    ChatMessage query("/users");
    _socket.send_to(boost::asio::buffer(query.get_raw_data()), _receiver_endpoint);
    _socket.receive_from(boost::asio::buffer(query.get_raw_data()), _receiver_endpoint);
    return query.get_text();
}

bool Socket::send_message(const std::string &message) {
    ChatMessage msg(message);
    _socket.async_send_to(boost::asio::buffer(msg.get_raw_data()), _receiver_endpoint,
                          boost::bind(&Socket::handle_send, this, msg,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
    return true;
}

void Socket::set_username(std::string username) {
    _username = std::move(username);
}

std::pair<bool, std::string> Socket::handle_connect() {
    std::string connection_string = "/connect " + _username;
    ChatMessage initial(connection_string);
    _socket.send_to(boost::asio::buffer(initial.get_raw_data()), _receiver_endpoint);
    _socket.receive_from(boost::asio::buffer(initial.get_raw_data()), _receiver_endpoint);
    initial.decode();
    std::string data = initial.get_text();
    if (initial.get_type() == MessageType::UNKNOWN)
        return {false, ""};
    if (data != "Ok")
        return {false, data};
    else
        return {true, ""};
}

void Socket::handle_send(ChatMessage &msg,
                         const boost::system::error_code & /*error*/,
                         std::size_t /*bytes_transferred*/) {
    //std::cout << "Message: " + *message << '\n';
}

void Socket::start_receive() {
    _socket.async_receive_from(boost::asio::buffer(_chat_message.get_raw_data()), _temp_endpoint,
                               boost::bind(&Socket::handle_receive,
                                           this, boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
}

void Socket::handle_receive(const boost::system::error_code &error,
                            std::size_t /*bytes_transferred*/) {
    if (!error || error == boost::asio::error::message_size) {
        _chat_message.decode();
        _out << _chat_message.get_text() << '\n';
    }
    start_receive();
}