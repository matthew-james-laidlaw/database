#include <client.h>
#include <logger.h>

static Log::Logger g_logger(L"client", Log::Color::Red);

Client::Client()
{
    auto wsa_data = WSADATA{};

    auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0)
    {
        g_logger.Error(L"WSAStartup failed with error code '{}'", result);
        throw std::runtime_error("WSAStartup failed");
    }

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
    {
        g_logger.Error(L"socket failed with error code '{}'", WSAGetLastError());
        throw std::runtime_error("socket failed");
    }

    PCSTR addr = "127.0.0.1";
    u_short port = 8080;

    auto server_address = sockaddr_in{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    // convert IP address from text to binary form
    inet_pton(AF_INET, addr, &server_address.sin_addr);

    std::wstring wide_addr(addr, addr + std::strlen(addr));
    g_logger.Info(L"attempting connection to address '{}'", wide_addr);

    result = connect(m_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
    if (result == SOCKET_ERROR)
    {
        g_logger.Error(L"connect failed with error code '{}'", WSAGetLastError());
        throw std::runtime_error("connect failed");
    }

    g_logger.Info(L"connected to server at address '{}'", wide_addr);
}

Client::~Client()
{
    closesocket(m_socket);
    WSACleanup();
    g_logger.Info(L"closed connection");
}

auto Client::Send(std::vector<char> const& payload) const -> void
{
    auto bytes_to_send = payload.size();
    auto total_bytes_sent = 0;

    while (total_bytes_sent < bytes_to_send)
    {
        auto bytes_sent = send(m_socket, payload.data() + total_bytes_sent, bytes_to_send - total_bytes_sent, 0);
        if (bytes_sent == SOCKET_ERROR)
        {
            g_logger.Error(L"send failed with error code '{}'", WSAGetLastError());
            throw std::runtime_error("send failed");
        }
        total_bytes_sent += bytes_sent;
    }

    std::wstring sent(payload.begin(), payload.end());
    g_logger.Info(L"sent message: '{}'", sent);
}

auto Client::Receive(size_t amount) const -> std::vector<char>
{
    std::vector<char> buffer(amount);

    int bytes_received = recv(m_socket, buffer.data(), buffer.size(), 0);
    if (bytes_received == SOCKET_ERROR)
    {
        g_logger.Error(L"recv failed with error code '{}'", WSAGetLastError());
        throw std::runtime_error("recv failed");
    }

    std::wstring received(buffer.data(), buffer.data() + bytes_received);
    g_logger.Info(L"received message: '{}'", received);

    return buffer;
}
