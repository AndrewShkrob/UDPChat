#ifndef CLIENT_CLIENT_HPP
#define CLIENT_CLIENT_HPP

#include <memory>
#include "states/empty_state.hpp"
#include "socket.hpp"

class Client {
    friend class states::State;
public:
    explicit Client(std::ostream &out);

    template<class State>
    void set_state() {
        _current_state = std::unique_ptr<State>(new State(*this));
    }

    std::ostream &out() const;

    void help() const;

    void login(const std::string &username);

    void connect(const std::string &address, const std::string &port);

    void create_room(const std::string &room_name, const std::string &password);

    void join_room(const std::string &room_name, const std::string &password);

    void view_rooms() const;

    void invite_messaging(const std::string &username) const;

    void accept_messaging(const std::string &username) const;

    void reject_messaging(const std::string &username) const;

    void view_users() const;

    void exit();

    void send_message(const std::string &message) const;

    void unknown_command() const;

protected:
    std::ostream &_out;
    std::unique_ptr<states::State> _current_state;
    std::string _username;
    Socket _socket;
};

#endif //CLIENT_CLIENT_HPP
