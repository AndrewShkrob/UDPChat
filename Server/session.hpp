#ifndef SERVER_SESSION_HPP
#define SERVER_SESSION_HPP

#include <boost/asio.hpp>
#include "participant.hpp"
#include "room.hpp"

class Session
        : public Participant,
          public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::udp::socket socket, std::shared_ptr<Room> room)
            : _socket(std::move(socket)),
              _room(room) {
    }

    void start() {
        _room->join(shared_from_this());
        do_read_header();
    }

    void deliver(const chat_message &msg) {
        bool write_in_progress = !_write_msgs.empty();
        _write_msgs.push_back(msg);
        if (!write_in_progress) {
            do_write();
        }
    }

private:
    void do_read_header() {
        auto self(shared_from_this());
        boost::asio::async_read(_socket,
                                boost::asio::buffer(_read_msg.data(), chat_message::header_length),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                    if (!ec && _read_msg.decode_header()) {
                                        do_read_body();
                                    } else {
                                        _room->leave(shared_from_this());
                                    }
                                });
    }

    void do_read_body() {
        auto self(shared_from_this());
        boost::asio::async_read(_socket,
                                boost::asio::buffer(_read_msg.body(), _read_msg.body_length()),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                    if (!ec) {
                                        _room->deliver(_read_msg);
                                        do_read_header();
                                    } else {
                                        _room->leave(shared_from_this());
                                    }
                                });
    }

    void do_write() {
        auto self(shared_from_this());
        boost::asio::async_write(_socket,
                                 boost::asio::buffer(_write_msgs.front().data(),
                                                     _write_msgs.front().length()),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (!ec) {
                                         _write_msgs.pop_front();
                                         if (!_write_msgs.empty()) {
                                             do_write();
                                         }
                                     } else {
                                         _room->leave(shared_from_this());
                                     }
                                 });
    }

    boost::asio::ip::tcp::socket _socket;
    std::shared_ptr<Room> _room;
    chat_message _read_msg;
    message_queue _write_msgs;
};

#endif //SERVER_SESSION_HPP
