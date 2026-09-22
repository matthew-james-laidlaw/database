#include <database.h>

#include <server.h>

#include <string>
#include <iostream>
#include <vector>
#include <filesystem>

static const char OP_INSERT = 0x00;
static const char OP_SELECT = 0x01;
static const char OP_UPDATE = 0x02;
static const char OP_DELETE = 0x03;

template <typename T>
auto ToBytes(T const& value) -> std::vector<char>
{
    auto bytes = std::vector<char>(sizeof(T));
    std::memcpy(bytes.data(), reinterpret_cast<char const*>(&value), sizeof(T));
    return bytes;
}

template <typename T>
auto Deserialize(std::vector<char> const& bytes) -> T
{
    T value;
    std::memcpy(&value, bytes.data(), bytes.size());
    return value;
}

class DatabaseServer
{
private:

    Database m_database;
    Server m_server;

public:

    DatabaseServer()
        : m_database(std::filesystem::current_path())
    {}

    auto Receive() -> void
    {
        auto op = m_server.Receive(1);
        switch (op[0])
        {
        case OP_INSERT: Insert(); break;
        case OP_SELECT: Select(); break;
        case OP_UPDATE: Update(); break;
        case OP_DELETE: Delete(); break;
        default: throw std::runtime_error("");
        }
    }

private:

    auto Insert() -> void
    {
        auto key_len_bytes = m_server.Receive(sizeof(uint32_t));
        auto key_len = Deserialize<uint32_t>(key_len_bytes);
        auto key_bytes = m_server.Receive(key_len);
        
        auto value_len_bytes = m_server.Receive(sizeof(uint32_t));
        auto value_len = Deserialize<uint32_t>(value_len_bytes);
        auto value_bytes = m_server.Receive(value_len);

        std::string key(key_bytes.begin(), key_bytes.end());
        std::string value(key_bytes.begin(), key_bytes.end());
    }

    auto Select() -> void
    {
        auto key_len_bytes = m_server.Receive(sizeof(uint32_t));
        auto key_len = Deserialize<uint32_t>(key_len_bytes);
        auto key_bytes = m_server.Receive(key_len);

        std::string key(key_bytes.begin(), key_bytes.end());
    }

    auto Update() -> void
    {
        auto key_len_bytes = m_server.Receive(sizeof(uint32_t));
        auto key_len = Deserialize<uint32_t>(key_len_bytes);
        auto key_bytes = m_server.Receive(key_len);
        
        auto value_len_bytes = m_server.Receive(sizeof(uint32_t));
        auto value_len = Deserialize<uint32_t>(value_len_bytes);
        auto value_bytes = m_server.Receive(value_len);

        std::string key(key_bytes.begin(), key_bytes.end());
        std::string value(key_bytes.begin(), key_bytes.end());
    }

    auto Delete() -> void
    {
        auto key_len_bytes = m_server.Receive(sizeof(uint32_t));
        auto key_len = Deserialize<uint32_t>(key_len_bytes);
        auto key_bytes = m_server.Receive(key_len);

        std::string key(key_bytes.begin(), key_bytes.end());
    }

};

auto Main() -> void
{
    auto server = DatabaseServer();
    server.Receive();
}

auto main() -> int
{
    try
    {
        Main();
        return 0;
    }
    catch(std::exception const& e)
    {
        return 1;
    }
}
