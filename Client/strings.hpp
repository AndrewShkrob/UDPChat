#ifndef CLIENT_STRINGS_HPP
#define CLIENT_STRINGS_HPP

#include <iostream>
#include <string_view>

namespace strings {
    namespace colors {
        constexpr std::string_view def = "\033[0m";
        constexpr std::string_view black = "\033[30m";
        constexpr std::string_view red = "\033[31m";
        constexpr std::string_view green = "\033[32m";
        constexpr std::string_view yellow = "\033[33m";
        constexpr std::string_view blue = "\033[34m";
        constexpr std::string_view magenta = "\033[35m";
        constexpr std::string_view cyan = "\033[36m";
        constexpr std::string_view white = "\033[37m";
    }

    constexpr std::string_view welcome = "\033[31mWelcome to the UDP Chat!\033[0m\n";
    constexpr std::string_view help_info = "\033[34mYou can use /help to see all available commands\033[0m\n";
    constexpr std::string_view help =
            "\033[34m"
            "/help - shows all available commands\n"
            "/login [username] - sets your login (to determine who sent the message)\n"
            "/connect [host:port] - connects to the server\n"
            "    example: /connect 127.0.0.1:8001\n"
            "/create_room [room_name] [password] - creates chat room on the server\n"
            "                                      password field may be empty\n"
            "    example: /create_room room1 1234\n"
            "             /create_room room2\n"
            "/join_room [room_name] [password] - joins to the chat room on the server\n"
            "                                    password if need\n"
            "    example: /join_room room1 1234\n"
            "             /join_room room2\n"
            "/rooms - all rooms, available on the server\n"
            "/invite_messaging [username] - sends invite to user to start chatting\n"
            "    example: /invite_messaging user2\n"
            "/accept_messaging [username] - accepts invite from username\n"
            "    example: /accept_messaging user1\n"
            "/users - all users, connected to the server\n"
            "/exit - exit from the server and close the application"
            "\033[0m\n";

    namespace error {
        void connection_failed(std::ostream &out, const std::string_view &ip, unsigned int port) {
            out << strings::colors::red << "Connection to server " << ip << ":" << port << " failed"
                << strings::colors::def << std::endl;
        }

        void cannot_create_room_not_connected(std::ostream &out) {
            out << strings::colors::red << "Cannot create room. You are not connected to the server"
                << strings::colors::def << std::endl;
        }

        void cannot_join_room_not_connected(std::ostream &out) {
            out << strings::colors::red << "Cannot join room. You are not connected to the server"
                << strings::colors::def << std::endl;
        }

        void cannot_view_rooms_not_connected(std::ostream &out) {
            out << strings::colors::red << "Cannot view rooms. You are not connected to the server"
                << strings::colors::def << std::endl;
        }

        void cannot_invite_messaging_not_connected(std::ostream &out) {
            out << strings::colors::red << "Cannot invite messaging. You are not connected to the server"
                << strings::colors::def << std::endl;
        }

        void cannot_accept_messaging_not_connected(std::ostream &out) {
            out << strings::colors::red << "Cannot accept messaging. You are not connected to the server"
                << strings::colors::def << std::endl;
        }

        void cannot_view_users_not_connected(std::ostream &out) {
            out << strings::colors::red << "Cannot view users. You are not connected to the server"
                << strings::colors::def << std::endl;
        }

        void cannot_send_message_not_connected(std::ostream &out) {
            out << strings::colors::red << "Cannot send message. You are not connected to the server"
                << strings::colors::def << std::endl;
        }
    }

    namespace success {
        void username_changed(std::ostream &out, const std::string_view &new_username) {
            out << strings::colors::yellow << "Username changed to " << new_username << strings::colors::def
                << std::endl;
        }

        void connected_to_server(std::ostream &out, const std::string_view &ip, unsigned int port) {
            out << strings::colors::yellow << "Successfully connected to server " << ip << ":" << port
                << strings::colors::def << std::endl;
        }
    }
}

#endif //CLIENT_STRINGS_HPP
