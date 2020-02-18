cmake_minimum_required(VERSION 3.16)
project(Server)

set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system thread date_time)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(Server
        main.cpp
        chat_message.hpp
        server.hpp
        room.hpp
        participant.hpp
        session.hpp
        )
target_link_libraries(Server ${Boost_LIBRARIES})