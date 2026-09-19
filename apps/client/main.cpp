#include <logger.h>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <format>
#include <stdexcept>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
private:

    SOCKET m_socket;

public:

    Client()
    {
        WSADATA wsa_data{};

        auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (result != 0)
        {
            Log::Error(L"WSAStartup failed with error code '{}'", result);
            throw std::runtime_error("");
        }

        m_socket = socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
        );

        if (m_socket == INVALID_SOCKET)
        {
            Log::Error(L"socket failed with error code '{}'", WSAGetLastError());
            WSACleanup();
            throw std::runtime_error("");
        }

        PCSTR addr = "127.0.0.1";
        u_short port = 8080;

        auto server_address = sockaddr_in{};
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);

        inet_pton(
            AF_INET,
            addr,
            &server_address.sin_addr
        );

        result = connect(
            m_socket,
            reinterpret_cast<sockaddr*>(&server_address),
            sizeof(server_address)
        );

        if (result == SOCKET_ERROR)
        {
            Log::Error(L"connect failed with error code '{}'", WSAGetLastError());
            throw std::runtime_error("");
        }

        std::wstring wide_addr(addr, addr + std::strlen(addr));
        Log::Info(L"Connected to server at address '{}'", wide_addr);
    }

    ~Client()
    {
        closesocket(m_socket);
        WSACleanup();
    }

    auto Send(char const* message) -> void
    {
        int bytes_sent = send(
            m_socket,
            message,
            sizeof(message) - 1,
            0
        );

        if (bytes_sent == SOCKET_ERROR)
        {
            Log::Error(L"send failed with error code '{}'", WSAGetLastError());
            throw std::runtime_error("");
        }
    }

    auto Receive() -> void
    {
        char buffer[4096];

        int bytes_received = recv(
            m_socket,
            buffer,
            sizeof(buffer),
            0
        );

        if (bytes_received == SOCKET_ERROR)
        {
            Log::Error(L"recv failed with error code '{}'", WSAGetLastError());
            throw std::runtime_error("");
        }

        std::wstring result(buffer, buffer + bytes_received);
        Log::Info(L"Server response: {}", result);
    }

};

int main()
{
    auto client = Client();

    const char message[] = "Hello from client!";
    client.Send(message);
    client.Receive();

    return 0;
}
