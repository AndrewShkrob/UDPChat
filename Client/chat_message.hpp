#ifndef CLIENT_CHAT_MESSAGE_HPP
#define CLIENT_CHAT_MESSAGE_HPP

#include <boost/array.hpp>
#include <cstring>
#include <ctime>

class ChatMessage;

enum class MessageType {
    kOnlineState = 0,
    kSimpleMessage,
    kUnknown = 100,
};

class ChatMessage {
public:

    static constexpr size_t kMessageSize = 1025;

    ChatMessage() : data_(), type_(MessageType::kUnknown) {
        data_[0] = static_cast<char>(MessageType::kUnknown);
    }

    explicit ChatMessage(std::time_t time) : data_(), type_(MessageType::kOnlineState) {
        WriteToMessage(time);
    }

    explicit ChatMessage(char data[1025]) : data_(), type_(MessageType::kUnknown) {
        data_[0] = static_cast<char>(MessageType::kUnknown);
        memcpy(data_.data(), data, sizeof(char) * 1025);
        Decode();
    }

    explicit ChatMessage(const std::string &textMessage) : data_(), type_(MessageType::kSimpleMessage) {
        data_[0] = static_cast<char>(MessageType::kSimpleMessage);
        for (size_t i = 0; i < std::max(kMessageSize - 1, textMessage.size()); ++i) {
            data_[i + 1] = textMessage[i];
        }
    }

    void Decode() {
        switch (static_cast<char>(data_[0])) {
            case 0: {
                type_ = MessageType::kOnlineState;
                return;
            }
            case 1: {
                type_ = MessageType::kSimpleMessage;
                return;
            }
            default: {
                type_ = MessageType::kUnknown;
                return;
            }
        }
    }

    [[nodiscard]] MessageType GetType() const {
        return type_;
    }

    boost::array<char, kMessageSize> &GetRawData() {
        return data_;
    }

    template<typename T>
    void WriteToMessage(const T &data) {
        const char *begin = reinterpret_cast<const char *>(std::addressof(data));
        const char *end = begin + sizeof(T);
        std::copy(begin, end, data_.begin() + 1);
    }

    void WriteToMessage(const std::string &data) {
        type_ = MessageType::kSimpleMessage;
        data_[0] = static_cast<char>(MessageType::kSimpleMessage);
        for (size_t i = 0; i < std::max(kMessageSize - 1, data.size()); ++i) {
            data_[i + 1] = data[i];
        }
    }

    template<typename T>
    T ReadFromMessage() const {
        T ret;
        char *begin = reinterpret_cast<char *>(std::addressof(ret));
        std::copy(data_.begin() + 1, data_.begin() + 1 + sizeof(T), begin);
        return ret;
    }

    [[nodiscard]] std::string ReadText() const {
        if (type_ != MessageType::kSimpleMessage) {
            throw;
        }
        std::string ret{};
        for (size_t i = 1; i < kMessageSize && data_[i] != 0; ++i) {
            ret.push_back(data_[i]);
        }
        return ret;
    }


private:
    MessageType type_;
    boost::array<char, kMessageSize> data_;

};

#endif //CLIENT_CHAT_MESSAGE_HPP
