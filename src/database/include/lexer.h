#pragma once

#include <token.h>

#include <string_view>
#include <vector>

auto Lex(std::string_view& source) -> std::vector<Token>;
