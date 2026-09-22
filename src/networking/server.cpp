#include <logger.h>
#include <server.h>

static Log::Logger g_logger(L"server", Log::Color::Blue);

Server::Server()
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

    m_client = accept(m_server, nullptr, nullptr);
    if (m_client == INVALID_SOCKET)
    {
        g_logger.Error(L"accept failed with error code '{}'", WSAGetLastError());
        throw std::runtime_error("accept failed");
    }
    g_logger.Info(L"accepted client connection");
}

Server::~Server()
{
    closesocket(m_client);
    closesocket(m_server);
    WSACleanup();
    g_logger.Info(L"stopped listening");
}

auto Server::Send(std::vector<char> const& payload) const -> void
{
    auto bytes_to_send = payload.size();
    auto total_bytes_sent = 0;

    while (total_bytes_sent < bytes_to_send)
    {
        auto bytes_sent = send(m_client, payload.data() + total_bytes_sent, bytes_to_send - total_bytes_sent, 0);
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

auto Server::Receive(size_t amount) const -> std::vector<char>
{
    std::vector<char> buffer(amount);

    int bytes_received = recv(m_client, buffer.data(), buffer.size(), 0);
    if (bytes_received == SOCKET_ERROR)
    {
        g_logger.Error(L"recv failed with error code '{}'", WSAGetLastError());
        throw std::runtime_error("recv failed");
    }

    std::wstring received(buffer.data(), buffer.data() + bytes_received);
    g_logger.Info(L"received message: '{}'", received);

    return buffer;
}
