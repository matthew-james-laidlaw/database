#pragma once

#include <print>

template <typename... Ts>
auto LogInfo(std::format_string<Ts...> msg, Ts&&... args) -> void
{
	std::print("[info] ");
	std::println(msg, std::forward<Ts>(args)...);
}

template <typename... Ts>
auto LogFatal(std::format_string<Ts...> msg, Ts&&... args) -> void
{
	std::print("[fatal] ");
	std::println(msg, std::forward<Ts>(args)...);
	std::exit(1);
}
