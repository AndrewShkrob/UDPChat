#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "room.hpp"
#include "session.hpp"

class Server {
public:
    explicit Server(unsigned short port)
            : _socket(_io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)) {
        start_receive();
        _io_service.run();
    }

protected:
    void start_receive() {
        _socket.async_receive_from(
                boost::asio::buffer(_chat_message.data(), _chat_message.header_length), _remote_endpoint,
                boost::bind(&Server::handle_receive, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code &error,
                        std::size_t /*bytes_transferred*/) {
        if (!error || error == boost::asio::error::message_size) {
            std::string username = _chat_message.body();
            std::cout << "New connection from " << _remote_endpoint << ". Username: " << username << '\n';
            _connected_users[username] =
                    std::make_shared<Session>(boost::asio::ip::udp::socket(_remote_endpoint), nullptr);
            start_receive();
        }
    }

private:
    boost::asio::io_service _io_service;
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _remote_endpoint;

    chat_message _chat_message;

    std::unordered_map<std::string, std::shared_ptr<Session>> _connected_users;
    std::unordered_map<std::string, Room> _rooms;
};

#endif //SERVER_SERVER_HPP
