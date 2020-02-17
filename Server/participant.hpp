#ifndef SERVER_PARTICIPANT_HPP
#define SERVER_PARTICIPANT_HPP

#include <memory>
#include "chat_message.hpp"

class Participant {
public:
    virtual ~Participant() {}

    virtual void deliver(const chat_message &msg) = 0;
};

typedef std::shared_ptr<Participant> participant_ptr;

#endif //SERVER_PARTICIPANT_HPP
