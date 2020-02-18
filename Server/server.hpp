#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include "room.hpp"
#include "session.hpp"

using Connections = std::set<boost::asio::ip::udp::endpoint>;
using EndpointRef = std::reference_wrapper<const boost::asio::ip::udp::endpoint>;

class Server {
public:
    explicit Server(unsigned short port)
            : _socket(_io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)) {
        start_receive();
        _io_context.run();
    }

protected:
    void start_receive() {
        _socket.async_receive_from(
                boost::asio::buffer(_chat_message.get_raw_data(), ChatMessage::MESSAGE_SIZE),
                _remote_endpoint,
                boost::bind(
                        &Server::handle_receive,
                        this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred
                )
        );
    }

    void handle_receive(const boost::system::error_code &error,
                        std::size_t /*bytes_transferred*/) {
        if (!error || error == boost::asio::error::message_size) {
            _chat_message.decode();
            if (_chat_message.get_type() == MessageType::CONNECT) {
                handle_new_connection();
            } else if (_chat_message.get_type() == MessageType::MESSAGE) {
                handle_message();
            } else if (_chat_message.get_type() == MessageType::COMMAND) {
                handle_command();
            }
            start_receive();
        }
    }

    void handle_new_connection() {
        std::string username = _chat_message.get_text();
        username = username.substr(username.find(' ') + 1, username.size() - username.find(' '));
        std::cout << "[SERVER] Client with login \"" << username << "\" tries to establish connection." << std::endl;
        if (_connected_users.find(_remote_endpoint) != std::end(_connected_users)) {
            std::cout << "[SERVER] Error! Client with such login already connected" << std::endl;
            Session tmp(_socket, _remote_endpoint, "");
            tmp.deliver(ChatMessage("User with such login already exists."));
        } else {
            auto pos = _connections.insert(std::move(_remote_endpoint)).first;
            _connected_users[*pos] =
                    std::make_shared<Session>(_socket, *pos, username);
            _connected_users[*pos]->deliver(ChatMessage("Ok"));
            std::cout << "[SERVER] Client \"" << username << "\" connected." << std::endl;
        }
    }

    void handle_message() {
        auto session = _connected_users[_remote_endpoint];
        auto room = session->get_room();
        if (!room)
            return;
        std::string username = session->get_username();
        std::string curr_time = boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
        std::string message = "\033[35m[" + username + "]"
                              + " \033[36m[" + curr_time + "]\033[0m "
                              + _chat_message.get_text();
        std::cout << message << std::endl;
        std::for_each(std::begin(_connected_users), std::end(_connected_users), [&message](const auto &param) {
            param.second->deliver(ChatMessage(message));
        });
    }

    void handle_command() {
        auto session = _connected_users[_remote_endpoint];
        std::string username = session->get_username();
        std::string command = _chat_message.get_text();
        std::cout << "[SERVER] Client \"" << username << "\" sent command: " << command << std::endl;
        if (command == "/users") {
            session->deliver(ChatMessage(get_users(username)));
        } else if (command.starts_with("/create_room")) {
            session->deliver(ChatMessage(create_room(username)));
        }
    }

    std::string get_users(const std::string &current_user) {
        std::string users;
        for (const auto &[endp, session] : _connected_users)
            if (session->get_username() != current_user)
                users += session->get_username() + "\n";
        return users;
    }

    std::string create_room(const std::string &current_user) {
        std::istringstream is(_chat_message.get_text());
        std::string command;
        std::string room_name;
        std::string password;
        is >> command >> room_name >> password;
        if (_rooms.find(room_name) != std::end(_rooms))
            return "Such room already exists.";
        _rooms[room_name] = std::make_shared<Room>(room_name, password);
        std::cout << "[SERVER] Client \"" << current_user << "\" created room with name \"" << room_name
                  << "\" and password \"" << password << "\"" << std::endl;
        return "Ok";
    }

private:
    boost::asio::io_context _io_context;
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _remote_endpoint;

    ChatMessage _chat_message;

    Connections _connections;
    std::map<EndpointRef, std::shared_ptr<Session>> _connected_users;
    std::unordered_map<std::string, std::shared_ptr<Room>> _rooms;
};

#endif //SERVER_SERVER_HPP
