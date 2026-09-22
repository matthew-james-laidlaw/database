#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define LOGGER_DEFINED_LEAN_AND_MEAN
#endif

#include <Windows.h>

#ifdef LOGGER_DEFINED_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef LOGGER_DEFINED_LEAN_AND_MEAN
#endif

#include <format>
#include <iostream>
#include <unordered_map>

namespace Log
{
    
inline auto GetMutex() -> HANDLE
{
    static auto mutex = []() -> HANDLE
    {
        auto handle = CreateMutexW(nullptr, false, L"Global\\DatabaseLogger");
        if (!handle)
        {
            throw std::runtime_error(std::format("CreateMutexW failed with error code '{}'", GetLastError()));
        }
        return handle;
    }();
    return mutex;
}

class Lock
{
public:

    Lock()
    {
        auto result = WaitForSingleObject(GetMutex(), INFINITE);
        if (result != WAIT_OBJECT_0 && result != WAIT_ABANDONED)
        {
            throw std::runtime_error(std::format("WaitForSingleObject failed with error code '{}'", GetLastError()));
        }
    }

    ~Lock()
    {
        ReleaseMutex(GetMutex());
    }

    Lock(Lock const&) = delete;
    Lock& operator=(Lock const&) = delete;

};

enum class Color
{
    Red,
    Green,
    Blue,
    Yellow,
    Reset
};

inline std::unordered_map<Color, char const*> Colors = {
    { Color::Reset, "\033[0m" },
    { Color::Red, "\033[31m" },
    { Color::Green, "\033[32m" },
    { Color::Blue, "\033[34m" },
    { Color::Yellow, "\033[33m" },
};

struct Logger
{
private:

    std::wstring m_name;
    Color m_color;

public:

    Logger(std::wstring const& name, Color color)
        : m_name(name), m_color(color)
    {}

    template <typename... Ts>
    auto Info(std::wformat_string<Ts...> msg, Ts&&... args) -> void
    {
        auto lock = Lock();
        std::wcout << Colors[m_color] << std::format(L"[{}][info] ", m_name) << std::format(msg, std::forward<Ts>(args)...) << Colors[Color::Reset] << L'\n';
    }

    template <typename... Ts>
    auto Info(std::format_string<Ts...> msg, Ts&&... args) -> void
    {
        auto lock = Lock();
        auto name = std::string(m_name.begin(), m_name.end());
        std::cout << Colors[m_color] << std::format("[{}][info] ", name) << std::format(msg, std::forward<Ts>(args)...) << Colors[Color::Reset] << '\n';
    }

    template <typename... Ts>
    auto Error(std::wformat_string<Ts...> msg, Ts&&... args) -> void
    {
        auto lock = Lock();
        std::wcout << Colors[m_color] << std::format(L"[{}][error] ", m_name) << std::format(msg, std::forward<Ts>(args)...) << Colors[Color::Reset] << L'\n';
    }

    template <typename... Ts>
    auto Error(std::format_string<Ts...> msg, Ts&&... args) -> void
    {
        auto lock = Lock();
        auto name = std::string(m_name.begin(), m_name.end());
        std::cout << Colors[m_color] << std::format("[{}][error] ", name) << std::format(msg, std::forward<Ts>(args)...) << Colors[Color::Reset] << '\n';
    }

};

} // namespace Log
