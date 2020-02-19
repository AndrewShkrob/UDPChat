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
        std::string room_name = room->get_name();
        std::string message = "\033[35m[" + username + "]"
                              + " \033[36m[" + curr_time + "]"
                              + " \033[32m[" + room_name + "]\033[0m "
                              + _chat_message.get_text();
        std::string message_for_users = "\033[35m[" + username + "]"
                                        + " \033[36m[" + curr_time + "]\033[0m "
                                        + _chat_message.get_text();
        std::cout << message << std::endl;
        room->deliver(ChatMessage(message_for_users));
    }

    void handle_command() {
        auto session = _connected_users[_remote_endpoint];
        std::string username = session->get_username();
        std::string command = _chat_message.get_text();
        std::cout << "[SERVER] Client \"" << username << "\" sent command: " << command << std::endl;
        if (command == "/users") {
            session->deliver(ChatMessage(get_users(username)));
        } else if (command == "/rooms") {
            session->deliver(ChatMessage(get_rooms()));
        } else if (command.starts_with("/create_room")) {
            session->deliver(ChatMessage(create_room(username)));
        } else if (command.starts_with("/exit_room")) {
            exit_room();
        } else if (command.starts_with("/join_room")) {
            const auto &[status, message] = join_room(username);
            session->deliver(ChatMessage(message));
            // костыль, чтобы получать последние сообщения при подключении к чату
            if (status) {
                session->get_room()->deliver_first(session);
            }
        } else if (command.starts_with("/invite_messaging")) {
            invite_messaging();
        }
    }

    std::string get_users(const std::string &current_user) {
        std::string users;
        for (const auto &[_, session] : _connected_users)
            if (session->get_username() != current_user)
                users += session->get_username() + "\n";
        return users;
    }

    std::string get_rooms() {
        std::string rooms;
        for (const auto &[room_name, room] : _rooms)
            rooms += room_name + (!room->is_locked() ? "" : " (locked)") + "\n";
        return rooms;
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

    std::pair<bool, std::string> join_room(const std::string &user) {
        std::istringstream is(_chat_message.get_text());
        std::string command;
        std::string room_name;
        std::string password;
        is >> command >> room_name >> password;
        auto room_it = _rooms.find(room_name);
        if (room_it == std::end(_rooms))
            return {false, "No such room."};
        auto room = room_it->second;
        if (!room->validate_password(password) && room->is_locked())
            return {false, "Incorrect password."};
        room->join(_connected_users[_remote_endpoint]);
        _connected_users[_remote_endpoint]->set_room(room);
        std::cout << "[SERVER] Client \"" << user << "\" joins room with name \"" << room_name << "\"" << std::endl;
        return {true, "Ok"};
    }

    void exit_room() {
        auto user = _connected_users[_remote_endpoint];
        auto room = user->get_room();
        if (!room)
            return;
        std::string room_name = room->get_name();
        room->leave(user);
        user->set_room(nullptr);
        std::cout << "[SERVER] Client \"" << user->get_username() << "\" leaves room with name \"" << room_name << "\""
                  << std::endl;
    }

    void invite_messaging(const std::string &username) {
        std::istringstream is(_chat_message.get_text());
        std::string command;
        std::string invited_username;
        is >> command >> invited_username;
        bool user_exists = false;
        EndpointRef invited_endpoint = _remote_endpoint;
        for (const auto &[endp, session] : _connected_users)
            if (session->get_username() == invited_username) {
                user_exists = true;
                invited_endpoint = endp;
                break;
            }
        if (!user_exists)
            return;
        _invites_from_to.emplace(_remote_endpoint, invited_endpoint);
        _invites_to_from.emplace(invited_endpoint, _remote_endpoint);
        _connected_users[invited_endpoint]->deliver(ChatMessage(
                "\033[32m[INFO] User " + username + " invites you to private chat.\033[0m"));
        std::cout << "[SERVER] Client \"" << username << "\" invites \"" << invited_username << "\" to private chat"
                  << std::endl;
    }

private:
    boost::asio::io_context _io_context;
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _remote_endpoint;

    ChatMessage _chat_message;

    Connections _connections;
    std::map<EndpointRef, std::shared_ptr<Session>> _connected_users;
    std::unordered_map<std::string, std::shared_ptr<Room>> _rooms;
    std::multimap<EndpointRef, EndpointRef> _invites_from_to;
    std::multimap<EndpointRef, EndpointRef> _invites_to_from;
};

#endif //SERVER_SERVER_HPP
