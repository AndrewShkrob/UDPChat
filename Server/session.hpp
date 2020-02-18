#ifndef SERVER_SESSION_HPP
#define SERVER_SESSION_HPP

#include <boost/asio.hpp>
#include <utility>
#include "participant.hpp"
#include "room.hpp"

class Session
        : public Participant,
          public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::udp::socket &server_socket,
            const boost::asio::ip::udp::endpoint &remote_endpoint,
            std::string username)
            : Participant(std::move(username)),
              _server_socket(server_socket),
              _remote_endpoint(remote_endpoint) {}

    void deliver(const ChatMessage &msg) override {
        _write_msgs.push_back(msg);
        if (!_write_msgs.empty()) {
            send_message(_write_msgs.front());
        }
    }

    void set_room(std::shared_ptr<Room> room) {
        _room = std::move(room);
    }

    std::shared_ptr<Room> get_room() const {
        return _room;
    }

private:
    void send_message(ChatMessage &msg) {
        _server_socket.async_send_to(
                boost::asio::buffer(msg.get_raw_data()),
                _remote_endpoint,
                boost::bind(
                        &Session::handle_send,
                        this,
                        msg,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred
                )
        );
    }

    void handle_send(ChatMessage, const boost::system::error_code &error,
                     std::size_t /*bytes_transferred*/) {
        if (!error)
            _write_msgs.pop_front();
    }

    boost::asio::ip::udp::socket &_server_socket;
    const boost::asio::ip::udp::endpoint &_remote_endpoint;
    std::shared_ptr<Room> _room;
    MessageQueue _write_msgs;
};

#endif //SERVER_SESSION_HPP
