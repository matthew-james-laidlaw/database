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

template <typename... Ts>
auto Info(std::wformat_string<Ts...> msg, Ts&&... args) -> void
{
    auto lock = Lock();
    auto pid = GetCurrentProcessId();
    std::wcout << std::format(L"[info][{}] ", pid) << std::format(msg, std::forward<Ts>(args)...) << L'\n';
}

template <typename... Ts>
auto Error(std::wformat_string<Ts...> msg, Ts&&... args) -> void
{
    auto lock = Lock();
    auto pid = GetCurrentProcessId();
    std::wcout << std::format(L"[error][{}] ", pid) << std::format(msg, std::forward<Ts>(args)...) << L'\n';
}

} // namespace Log

