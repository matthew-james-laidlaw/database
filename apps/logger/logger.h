#pragma once

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define LOGGER_DEFINED_LEAN_AND_MEAN

#include <Windows.h>
#endif

#ifdef LOGGER_DEFINED_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef LOGGER_DEFINED_LEAN_AND_MEAN
#endif

#endif

#include <cerrno>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

#include <format>
#include <iostream>
#include <unordered_map>

namespace Log
{

class IProcessLock
{
public:

    virtual auto Acquire() -> void = 0;
    virtual auto Release() -> void = 0;
    virtual ~IProcessLock() = default;

};

#ifdef _WIN32
class WindowsProcessLock : public IProcessLock
{
private:

    bool m_acquired;

public:

    WindowsProcessLock()
        : m_acquired(false)
    {
        Acquire();
    }

    ~WindowsProcessLock()
    {
        Release();
    }

    WindowsProcessLock(WindowsProcessLock const&) = delete;
    WindowsProcessLock& operator=(WindowsProcessLock const&) = delete;

    auto Acquire() -> void override
    {
        auto result = WaitForSingleObject(GetMutex(), INFINITE);
        if (result != WAIT_OBJECT_0 && result != WAIT_ABANDONED)
        {
            throw std::runtime_error(std::format("WaitForSingleObject failed with error code '{}'", GetLastError()));
        }
        m_acquired = true;
    }

    auto Release() -> void override
    {
        if (m_acquired)
        {
            if (!ReleaseMutex(GetMutex()))
            {
                throw std::runtime_error(std::format("ReleaseMutex failed with error code '{}'", GetLastError()));
            }
            m_acquired = false;
        }
    }

private:

    static auto GetMutex() -> HANDLE
    {
        static auto mutex = CreateMutexW(nullptr, FALSE, L"Global\\DatabaseLogger");
        if (!mutex)
        {
            throw std::runtime_error(std::format("CreateMutexW failed with error code '{}'", GetLastError()));
        }
        return mutex;
    }

};
#endif

class UnixProcessLock : public IProcessLock
{
private:

    int m_fd;
    bool m_acquired;

public:

    UnixProcessLock()
        : m_fd(-1), m_acquired(false)
    {
        m_fd = open("/tmp/database-terminal.lock", O_CREAT | O_RDWR, 0666);
        if (m_fd == -1)
        {
            throw std::runtime_error("open lock file failed");
        }
        Acquire();
    }

    ~UnixProcessLock()
    {
        Release();
        if (m_fd != -1)
        {
            close(m_fd);
        }
    }

    UnixProcessLock(UnixProcessLock const&) = delete;
    UnixProcessLock& operator=(UnixProcessLock const&) = delete;

    auto Acquire() -> void override
    {
        while (flock(m_fd, LOCK_EX) == -1)
        {
            if (errno != EINTR)
            {
                throw std::runtime_error("flock failed");
            }
        }
        m_acquired = true;
    }

    auto Release() -> void override
    {
        if (m_acquired)
        {
            flock(m_fd, LOCK_UN);
            m_acquired = false;
        }
    }

};

#ifdef _WIN32
using Lock = WindowsProcessLock;
#else
using Lock = UnixProcessLock;
#endif

enum class Color
{
    Red,
    Green,
    Blue,
    Reset
};

std::unordered_map<Color, char const*> Colors = {
    { Color::Reset, "\033[0m" },
    { Color::Red, "\033[31m" },
    { Color::Green, "\033[32m" },
    { Color::Blue, "\033[34m" },
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
    auto Error(std::wformat_string<Ts...> msg, Ts&&... args) -> void
    {
        auto lock = Lock();
        std::wcout << Colors[m_color] << std::format(L"[{}][error] ", m_name) << std::format(msg, std::forward<Ts>(args)...) << Colors[Color::Reset] << L'\n';
    }

};

} // namespace Log
