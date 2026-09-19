#include <WinSock2.h>
#include <WS2tcpip.h>

#include <logger.h>

#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

Log::Logger g_logger(L"server");

struct Socket
{
private:

    SOCKET m_handle;

public:

    Socket(SOCKET handle)
        : m_handle(handle)
    {}

    ~Socket()
    {
        closesocket(m_handle);
    }

    auto Get() const -> SOCKET
    {
        return m_handle;
    }

    auto Get() -> SOCKET
    {
        return m_handle;
    }

};

class Server
{
private:

    SOCKET m_server;

public:

    Server()
    {
        auto wsa_data = WSADATA{};

        auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (result != 0)
        {
            g_logger.Error(L"WSAStartup failed with error code '{}'", result);
            throw std::runtime_error("WSAStartup failed");
        }

        m_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_server == INVALID_SOCKET)
        {
            g_logger.Error(L"socket failed with error code '{}'", WSAGetLastError());
            throw std::runtime_error("socket failed");
        }

        PCSTR addr = "127.0.0.1";
        u_short port = 8080;

        sockaddr_in server_address{};
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);

        // convert IP address from text to binary form
        inet_pton(AF_INET, addr, &server_address.sin_addr);

        result = bind(m_server, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
        if (result == SOCKET_ERROR)
        {
            g_logger.Error(L"bind failed with error code '{}'", WSAGetLastError());
            throw std::runtime_error("bind failed");
        }

        result = listen(m_server, SOMAXCONN);
        if (result == SOCKET_ERROR)
        {
            g_logger.Error(L"listen failed with error code '{}'", WSAGetLastError());
            throw std::runtime_error("listen failed");
        }

        std::wstring wide_addr(addr, addr + std::strlen(addr));
        g_logger.Info(L"listening on '{}'", wide_addr);
    }

    ~Server()
    {
        closesocket(m_server);
        WSACleanup();
    }

    auto Accept() -> Socket
    {
        auto client = accept(m_server, nullptr, nullptr);
        if (client == INVALID_SOCKET)
        {
            g_logger.Error(L"accept failed with error code '{}'", WSAGetLastError());
            throw std::runtime_error("accept failed");
        }
        g_logger.Info(L"accepted client connection");
        return Socket(client);
    }

    auto Send(Socket& client, char const* message) -> void
    {
        auto bytes_to_send = static_cast<int>(std::strlen(message));
        auto total_bytes_sent = 0;

        while (total_bytes_sent < bytes_to_send)
        {
            auto bytes_sent = send(client.Get(), message + total_bytes_sent, bytes_to_send - total_bytes_sent, 0);
            if (bytes_sent == SOCKET_ERROR)
            {
                g_logger.Error(L"send failed with error code '{}'", WSAGetLastError());
                throw std::runtime_error("send failed");
            }
            total_bytes_sent += bytes_sent;
        }

        std::wstring sent(message, message + total_bytes_sent);
        g_logger.Info(L"sent message: {}", sent);
    }

    auto Receive(Socket& client) -> void
    {
        char buffer[4096];

        int bytes_received = recv(client.Get(), buffer, sizeof(buffer), 0);
        if (bytes_received == SOCKET_ERROR)
        {
            g_logger.Error(L"recv failed with error code '{}'", WSAGetLastError());
            throw std::runtime_error("recv failed");
        }

        std::wstring received(buffer, buffer + bytes_received);
        g_logger.Info(L"received message: {}", received);
    }

};

int main()
{
    auto server = Server();

    auto client = server.Accept();
    server.Receive(client);
    server.Send(client, "Hello from server!");

    return 0;
}
