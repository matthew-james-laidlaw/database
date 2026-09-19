#pragma once

#include <cstdint>

struct Header
{
    size_t size;
};

struct Payload
{
    Header header;
    char const* data;
};

auto ToString(Header const& header) -> std::wstring
{
    return std::format(L"Header{{size={}}}", header.size);
}
