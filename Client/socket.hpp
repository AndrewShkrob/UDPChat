#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

//ChatClient(boost::asio::io_service& io_service, char* host, char* port) : socket_(io_service) {
//        udp::resolver resolver(io_service);
//        udp::resolver::query query(udp::v4(), host, port);
//        udp::resolver::iterator endpoint = resolver.resolve(query);
//        receiver_endpoint_ = *endpoint;
//        boost::asio::connect(socket_, endpoint);
//        handle_connect();
//        start_receive();
//        std::cout << "Established connection with " << receiver_endpoint_ << std::endl;
//}

class Socket : boost::noncopyable {
public:
    explicit Socket();

    bool connect(const std::string &ip, const std::string &port);

    bool disconnect();

    bool create_room(const std::string &room_name, const std::string &password);

    bool join_room(const std::string &room_name, const std::string &password);

    std::string get_rooms();

    bool invite_messaging(const std::string &username);

    bool accept_messaging(const std::string &username);

    std::string get_users();

    bool send_message(const std::string &message);

private:
    boost::asio::io_service _io_service;
    boost::asio::ip::udp::endpoint _temp_endpoint;
    boost::asio::ip::udp::endpoint _receiver_endpoint;
    boost::asio::ip::udp::socket _socket;

    boost::thread _thread;
};

#endif //CLIENT_SOCKET_HPP
