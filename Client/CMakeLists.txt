cmake_minimum_required(VERSION 3.16)
project(Client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(Client main.cpp chat_message.hpp strings.hpp client.hpp states/state.hpp states/empty_state.hpp connection.hpp)
target_link_libraries(Client ${Boost_LIBRARIES})