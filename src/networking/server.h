#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <vector>

#pragma comment(lib, "Ws2_32.lib")

class Server
{
private:

    SOCKET m_client;
    SOCKET m_server;

public:

    Server();
    ~Server();

    auto Send(std::vector<char> const& payload) const -> void;
    auto Receive(size_t amount) const -> std::vector<char>;

};
