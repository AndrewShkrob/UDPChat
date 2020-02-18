#ifndef SERVER_PARTICIPANT_HPP
#define SERVER_PARTICIPANT_HPP

#include <memory>
#include "chat_message.hpp"

class Participant {
public:
    Participant(std::string username) : _username(std::move(username)) {}

    virtual ~Participant() {}

    virtual void deliver(const ChatMessage &msg) = 0;

    std::string get_username() const {
        return _username;
    }

private:
    const std::string _username;
};

typedef std::shared_ptr<Participant> participant_ptr;

#endif //SERVER_PARTICIPANT_HPP
