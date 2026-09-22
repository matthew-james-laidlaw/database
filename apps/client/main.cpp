#include <client.h>
#include <logger.h>

#include <optional>
#include <string>
#include <iostream>

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

class DatabaseClient
{
private:

    Client m_client;

public:

	auto Insert(std::string const& key, std::string const& value) const -> int
    {
        auto key_bytes = std::vector<char>(key.begin(), key.end());
        auto value_bytes = std::vector<char>(value.begin(), value.end());
        
        m_client.Send({ OP_INSERT });
        m_client.Send(ToBytes<uint32_t>(key_bytes.size()));
        m_client.Send(key_bytes);
        m_client.Send(ToBytes<uint32_t>(value_bytes.size()));
        m_client.Send(value_bytes);

        return 0;
    }

	auto Select(std::string const& key) const -> std::optional<std::string>
    {
        return {};
    }

	auto Update(std::string const& key, std::string const& value) const -> int
    {
        return 0;
    }

	auto Delete(std::string const& key) const -> int
    {
        return 0;
    }

};

auto Main() -> void
{
    auto client = DatabaseClient();
    client.Insert("my-key", "my-value");
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
