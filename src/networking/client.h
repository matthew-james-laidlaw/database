#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <vector>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
private:

    SOCKET m_socket;

public:

    Client();
    ~Client();

    auto Send(std::vector<char> const& payload) const -> void;
    auto Receive(size_t amount) const -> std::vector<char>;

};
