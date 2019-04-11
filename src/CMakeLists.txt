cmake_minimum_required(VERSION 3.13)
project(src)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        src
        main.cpp
        argparse/argparse.hpp
        handshake/handshake.hpp
        handshake/handshake.cpp
        receiver/CReceiver.cpp
        receiver/CReceiver.hpp
        receiver/linux/receiver_socket.c
        receiver/linux/receiver_socket.h
        receiver/win/ReceiverSocket.c
        receiver/win/ReceiverSocket.h
        sender/CSender.cpp
        sender/CSender.hpp
        sender/linux/sender_socket.c
        sender/linux/sender_socket.h
        sender/win/SenderSocket.c
        sender/win/SenderSocket.h
        handshake/glob_consts.h
        file_lib/linux/file_lib.c
        file_lib/linux/file_lib.h
        file_lib/win/FileLib.c
        file_lib/win/FileLib.h
)
