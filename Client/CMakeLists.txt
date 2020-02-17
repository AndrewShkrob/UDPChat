cmake_minimum_required(VERSION 3.16)
project(Client)

set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS thread)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(Client main.cpp strings.hpp src/client.cpp client.hpp states/state.hpp states/empty_state.hpp states/src/empty_state.cpp states/src/state.cpp states/connected_state.hpp states/chating_state.hpp states/src/connected_state.cpp socket.hpp src/socket.cpp chat_message.hpp)
target_link_libraries(Client ${Boost_LIBRARIES})