#pragma once

#include <logger.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

class Server
{
public:

    Server()
    {
        WSADATA wsa_data{};
        int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (result != 0)
        {
            LogFatal("WSAStartup failed with code: '{}'", result);
        }

        SOCKET server_socket = socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
        );

        if (server_socket == INVALID_SOCKET)
        {
            LogFatal
        }
    }

};
