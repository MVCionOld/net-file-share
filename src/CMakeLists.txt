cmake_minimum_required(VERSION 3.13)
project(src)

set(CMAKE_CXX_STANDARD 14)

if (UNIX)
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
            sender/CSender.cpp
            sender/CSender.hpp
            sender/linux/sender_socket.c
            sender/linux/sender_socket.h
            handshake/glob_consts.h
            file_lib/linux/file_lib.c
            file_lib/linux/file_lib.h
            utils/CProgressBar.cpp
            utils/CProgressBar.hpp
            application/CApplication.cpp
            application/CApplication.hpp
    )
    target_link_libraries(src ${CMAKE_DL_LIBS})
    target_link_libraries(src pthread)
endif (UNIX)

if (WIN32)
    add_executable(
            src
            main.cpp
            argparse/argparse.hpp
            handshake/handshake.hpp
            handshake/handshake.cpp
            receiver/CReceiver.cpp
            receiver/CReceiver.hpp
            receiver/win/ReceiverSocket.c
            receiver/win/ReceiverSocket.h
            sender/CSender.cpp
            sender/CSender.hpp
            sender/win/SenderSocket.c
            sender/win/SenderSocket.h
            handshake/glob_consts.h
            file_lib/win/FileLib.c
            file_lib/win/FileLib.h
            utils/CProgressBar.cpp
            utils/CProgressBar.hpp
            application/CApplication.cpp application/CApplication.hpp
    )
    target_link_libraries(src ${CMAKE_DL_LIBS})
    target_link_libraries(src pthread)
    target_link_libraries(
            src
            wsock32
            ws2_32
            shlwapi
            advapi32
    )
endif (WIN32)