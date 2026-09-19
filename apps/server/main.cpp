#include <WinSock2.h>
#include <WS2tcpip.h>

#include <logger.h>

#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSADATA wsa_data{};

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET server_socket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (server_socket == INVALID_SOCKET)
    {
        std::cerr << "socket failed: " << WSAGetLastError() << '\n';
        WSACleanup();
        return 1;
    }

    PCSTR addr = "127.0.0.1";
    u_short port = 8080;

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    inet_pton(
        AF_INET,
        addr,
        &server_address.sin_addr
    );

    if (bind(
        server_socket,
        reinterpret_cast<sockaddr*>(&server_address),
        sizeof(server_address)
    ) == SOCKET_ERROR)
    {
        std::cerr << "bind failed: " << WSAGetLastError() << '\n';

        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "listen failed: " << WSAGetLastError() << '\n';

        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::wstring wide_addr(addr, addr + std::strlen(addr));
    Log::Info(L"Server listening on '{}'", wide_addr);

    SOCKET client_socket = accept(
        server_socket,
        nullptr,
        nullptr
    );

    if (client_socket == INVALID_SOCKET)
    {
        std::cerr << "accept failed: " << WSAGetLastError() << '\n';

        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    Log::Info(L"Client connected");

    char buffer[4096];

    while (true)
    {
        int bytes_received = recv(
            client_socket,
            buffer,
            sizeof(buffer),
            0
        );

        if (bytes_received == 0)
        {
            Log::Info(L"Client disconnected");
            break;
        }

        if (bytes_received == SOCKET_ERROR)
        {
            std::cerr << "recv failed: "
                << WSAGetLastError()
                << '\n';
            break;
        }

        std::wstring result(buffer, buffer + bytes_received);
        Log::Info(L"Received: {}", result);

        const char response[] = "Hello from server!";

        send(
            client_socket,
            response,
            sizeof(response) - 1,
            0
        );
    }

    closesocket(client_socket);
    closesocket(server_socket);

    WSACleanup();

    return 0;
}
