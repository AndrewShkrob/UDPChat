#ifndef CLIENT_CHAT_MESSAGE_HPP
#define CLIENT_CHAT_MESSAGE_HPP

#include <boost/array.hpp>
#include <cstring>
#include <ctime>

enum class MessageType {
    CONNECT = 0,
    COMMAND = 1,
    MESSAGE = 2,
    UNKNOWN = 3
};

class ChatMessage {
public:
    static constexpr size_t MESSAGE_SIZE = 1025;

    ChatMessage() : _type(MessageType::UNKNOWN) {}

    explicit ChatMessage(const std::string &text) {
        if (text.empty())
            _type = MessageType::UNKNOWN;
        else if (text.starts_with("/connect"))
            _type = MessageType::CONNECT;
        else if (text.starts_with('/'))
            _type = MessageType::COMMAND;
        else
            _type = MessageType::MESSAGE;
        _data[0] = static_cast<char>(_type);
        memcpy(_data.data() + 1, text.data(), sizeof(char) * std::min(MESSAGE_SIZE - 1, text.size()));
    }

    void decode() {
        switch (_data[0]) {
            case 0:
                _type = MessageType::CONNECT;
                return;
            case 1:
                _type = MessageType::COMMAND;
                return;
            case 2:
                _type = MessageType::MESSAGE;
                return;
            default:
                _type = MessageType::UNKNOWN;
                return;
        }
    }

    [[nodiscard]] MessageType get_type() const {
        return _type;
    }

    boost::array<char, MESSAGE_SIZE> &get_raw_data() {
        return _data;
    }

    [[nodiscard]] std::string get_text() const {
        if (_type == MessageType::UNKNOWN) {
            throw;
        }
        std::string ret;
        for (size_t i = 1; i < MESSAGE_SIZE && _data[i] != 0; ++i) {
            ret.push_back(_data[i]);
        }
        return ret;
    }

private:
    MessageType _type;
    boost::array<char, MESSAGE_SIZE> _data{0};
};

#endif //CLIENT_CHAT_MESSAGE_HPP
