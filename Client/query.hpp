#ifndef CLIENT_QUERY_HPP
#define CLIENT_QUERY_HPP

#include <istream>
#include <sstream>
#include "client.hpp"

enum class QueryType {
    HELP,
    LOGIN,
    CONNECT,
    CREATE_ROOM,
    JOIN_ROOM,
    ROOMS,
    INVITE_MESSAGING,
    ACCEPT_MESSAGING,
    USERS,
    EXIT,
    MESSAGE,
    UNKNOWN
};

class Query {
public:

    friend std::istream &operator>>(std::istream &is, Query &query);

    void process(Client &client) {
        switch (_type) {
            case QueryType::HELP:
                client.help();
                break;
            case QueryType::LOGIN:
                client.login(param1);
                break;
            case QueryType::CONNECT:
                client.connect(param1, param2);
                break;
            case QueryType::CREATE_ROOM:
                client.create_room(param1, param2);
                break;
            case QueryType::JOIN_ROOM:
                client.join_room(param1, param2);
                break;
            case QueryType::ROOMS:
                client.view_rooms();
                break;
            case QueryType::INVITE_MESSAGING:
                client.invite_messaging(param1);
            case QueryType::ACCEPT_MESSAGING:
                client.accept_messaging(param1);
            case QueryType::USERS:
                client.view_users();
                break;
            case QueryType::EXIT:
                client.exit();
                break;
            case QueryType::MESSAGE:
                client.send_message(message);
                break;
            default:
                client.unknown_command();
        }
    }

private:
    QueryType _type;
    std::string message;
    std::string param1;
    std::string param2;
};

std::istream &operator>>(std::istream &is, Query &query) {
    std::string q;
    getline(is, q);
    if (q[0] != '/') {
        query._type = QueryType::MESSAGE;
        query.message = q;
        return is;
    }
    std::istringstream istr(q);
    std::string command;
    std::string param1;
    std::string param2;

    istr >> command >> param1 >> param2;

    if (command == "/help")
        query._type = QueryType::HELP;
    else if (command == "/login") {
        query._type = QueryType::LOGIN;
        query.param1 = param1;
    } else if (command == "/connect") {
        query._type = QueryType::CONNECT;
        query.param1 = param1;
        query.param2 = param2;
    } else if (command == "/create_room") {
        query._type = QueryType::CREATE_ROOM;
        query.param1 = param1;
        query.param2 = param2;
    } else if (command == "/join_room") {
        query._type = QueryType::JOIN_ROOM;
        query.param1 = param1;
        query.param2 = param2;
    } else if (command == "/rooms")
        query._type = QueryType::ROOMS;
    else if (command == "/invite_messaging") {
        query._type = QueryType::INVITE_MESSAGING;
        query.param1 = param1;
    } else if (command == "/accept_messaging") {
        query._type = QueryType::ACCEPT_MESSAGING;
        query.param1 = param1;
    } else if (command == "/users")
        query._type = QueryType::USERS;
    else if (command == "/exit")
        query._type = QueryType::EXIT;
    else
        query._type = QueryType::UNKNOWN;

    return is;
}


#endif //CLIENT_QUERY_HPP
