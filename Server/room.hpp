#ifndef SERVER_ROOM_HPP
#define SERVER_ROOM_HPP

#include <deque>
#include <set>
#include "chat_message.hpp"
#include "participant.hpp"

class Room {
public:
    Room(std::string name, std::string password, bool is_public = true)
            : _public(is_public),
              _name(std::move(name)),
              _password(std::move(password)),
              _locked_with_password(!_password.empty()) {}

    void join(const participant_ptr &participant) {
        _participants.insert(participant);
        for (auto msg: _recent_msgs)
            participant->deliver(msg);
    }

    void leave(const participant_ptr &participant) {
        _participants.erase(participant);
    }

    void deliver(const chat_message &msg) {
        _recent_msgs.push_back(msg);
        while (_recent_msgs.size() > max_recent_msgs)
            _recent_msgs.pop_front();

        for (const auto &participant: _participants)
            participant->deliver(msg);
    }

private:
    bool _public;

    std::string _name;
    std::string _password;
    bool _locked_with_password;

    std::set<participant_ptr> _participants;

    enum {
        max_recent_msgs = 100
    };

    message_queue _recent_msgs;
};

#endif //SERVER_ROOM_HPP
