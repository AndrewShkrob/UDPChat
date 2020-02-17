#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "chat_message.hpp"

class Socket : boost::noncopyable {
public:
    explicit Socket(std::ostream &os);

    ~Socket();

    void connect(const std::string &ip, const std::string &port);

    bool disconnect();

    bool create_room(const std::string &room_name, const std::string &password);

    bool join_room(const std::string &room_name, const std::string &password);

    std::string get_rooms();

    bool invite_messaging(const std::string &username);

    bool accept_messaging(const std::string &username);

    std::string get_users();

    bool send_message(const std::string &message);

    void set_username(std::string username);

protected:
    void handle_connect();

    void handle_send(ChatMessage &msg,
                     const boost::system::error_code & /*error*/,
                     std::size_t /*bytes_transferred*/);

    void start_receive();

    void handle_receive(const boost::system::error_code &error,
                        std::size_t /*bytes_transferred*/);

private:
    std::ostream &_out;
    std::string _username;

    boost::asio::io_service _io_service;

    boost::asio::ip::udp::endpoint _temp_endpoint;
    boost::asio::ip::udp::endpoint _receiver_endpoint;
    boost::asio::ip::udp::socket _socket;

    std::unique_ptr<boost::thread> _thread;

    ChatMessage _chat_message;
};

#endif //CLIENT_SOCKET_HPP
