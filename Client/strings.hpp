#ifndef CLIENT_STRINGS_HPP
#define CLIENT_STRINGS_HPP

#include <iostream>
#include <string_view>

namespace strings {
    namespace colors {
        static constexpr std::string_view def = "\033[0m";
        static constexpr std::string_view black = "\033[30m";
        static constexpr std::string_view red = "\033[31m";
        static constexpr std::string_view green = "\033[32m";
        static constexpr std::string_view yellow = "\033[33m";
        static constexpr std::string_view blue = "\033[34m";
        static constexpr std::string_view magenta = "\033[35m";
        static constexpr std::string_view cyan = "\033[36m";
        static constexpr std::string_view white = "\033[37m";
    }

    static constexpr std::string_view welcome = "\033[31mWelcome to the UDP Chat!\033[0m\n";
    static constexpr std::string_view help_info = "\033[34mYou can use /help to see all available commands\033[0m\n";
    static constexpr std::string_view help =
            "\033[34m"
            "/help - shows all available commands\n"
            "/login <username> - sets your login (to determine who sent the message)\n"
            "/connect <host> <port> - connects to the server\n"
            "    example: /connect 127.0.0.1 8080\n"
            "/create_room <room_name> [password] - creates chat room on the server\n"
            "                                      password field may be empty\n"
            "    example: /create_room room1 1234\n"
            "             /create_room room2\n"
            "/join_room <room_name> [password] - joins to the chat room on the server\n"
            "                                    password if need\n"
            "    example: /join_room room1 1234\n"
            "             /join_room room2\n"
            "/rooms - all rooms, available on the server\n"
            "/invite_messaging <username> - sends invite to user to start chatting\n"
            "    example: /invite_messaging user2\n"
            "/accept_messaging <username> - accepts invite from username\n"
            "    example: /accept_messaging user1\n"
            "/users - all users, connected to the server\n"
            "/exit - exit from the server and close the application"
            "\033[0m\n";
    static constexpr std::string_view unknown_command = "\033[31mUnknown command\033[0m";
    static constexpr std::string_view exit_the_chat = "\033[35mExiting the chat. Buy.\033[0m";
    static constexpr std::string_view exit_the_server = "\033[35mExiting the server. Buy.\033[0m";
    static constexpr std::string_view exit_the_room = "\033[35mExiting the room. Buy.\033[0m";

    static constexpr std::string_view view_users = "Users list:";
    static constexpr std::string_view view_rooms = "Rooms list:";

    namespace error {
        namespace not_connected {
            static inline void cannot_create_room(std::ostream &out) {
                out << strings::colors::red << "Cannot create room. You are not connected to the server"
                    << strings::colors::def << std::endl;
            }

            static inline void cannot_join_room(std::ostream &out) {
                out << strings::colors::red << "Cannot join room. You are not connected to the server"
                    << strings::colors::def << std::endl;
            }

            static inline void cannot_view_rooms(std::ostream &out) {
                out << strings::colors::red << "Cannot view rooms. You are not connected to the server"
                    << strings::colors::def << std::endl;
            }

            static inline void cannot_invite_messaging(std::ostream &out) {
                out << strings::colors::red << "Cannot invite messaging. You are not connected to the server"
                    << strings::colors::def << std::endl;
            }

            static inline void cannot_accept_messaging(std::ostream &out) {
                out << strings::colors::red << "Cannot accept messaging. You are not connected to the server"
                    << strings::colors::def << std::endl;
            }

            static inline void cannot_view_users(std::ostream &out) {
                out << strings::colors::red << "Cannot view users. You are not connected to the server"
                    << strings::colors::def << std::endl;
            }

            static inline void cannot_send_message(std::ostream &out) {
                out << strings::colors::red << "Cannot send message. You are not connected to the server"
                    << strings::colors::def << std::endl;
            }
        }

        namespace not_chatting {
            static inline void cannot_send_message(std::ostream &out) {
                out << strings::colors::red << "Cannot send message. You are not in the room"
                    << strings::colors::def << std::endl;
            }
        }

        namespace chatting {
            static inline void cannot_create_room(std::ostream &out) {
                out << strings::colors::red << "Cannot create room. You are in the room now"
                    << strings::colors::def << std::endl;
            }
        }

        static inline void
        connection_failed(std::ostream &out, const std::string_view &ip, const std::string_view &port,
                          const std::string_view &message) {
            out << strings::colors::red << "Connection to server " << ip << ":" << port << " failed. " << message
                << strings::colors::def << std::endl;
        }

        static inline void
        room_creation_failed(std::ostream &out, const std::string_view &room_name, const std::string_view &message) {
            out << strings::colors::red << "Something went wrong while creating room " << room_name << ". " << message
                << strings::colors::def << std::endl;
        }

        static inline void
        room_joining_failed(std::ostream &out, const std::string_view &room_name, const std::string_view &message) {
            out << strings::colors::red << "Something went wrong while joining room " << room_name << ". " << message
                << strings::colors::def << std::endl;
        }
    }

    namespace success {
        static inline void username_changed(std::ostream &out, const std::string_view &new_username) {
            out << strings::colors::yellow << "Username changed to " << new_username << strings::colors::def
                << std::endl;
        }

        static inline void connected_to_server(std::ostream &out, const std::string_view &ip, const std::string &port) {
            out << strings::colors::yellow << "Successfully connected to server " << ip << ":" << port
                << strings::colors::def << std::endl;
        }

        static inline void room_created(std::ostream &out, const std::string_view &room_name) {
            out << strings::colors::yellow << "Successfully created room " << room_name << strings::colors::def
                << std::endl;
        }

        static inline void joined_room(std::ostream &out, const std::string_view &room_name) {
            out << strings::colors::yellow << "Successfully joined room " << room_name << strings::colors::def
                << std::endl;
        }
    }
}


#endif //CLIENT_STRINGS_HPP
