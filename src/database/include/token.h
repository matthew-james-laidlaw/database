#pragma once

#include <string>

class Token
{
public:

	enum class Type
	{
		INSERT, SELECT, UPDATE, TOK_DELETE,
		LITERAL
	};

private:

	std::string m_lexeme;
	Type m_type;

public:

	Token(std::string lexeme, Type type);

	auto GetLexeme() const -> std::string;
	auto GetType() const -> Type;

};

auto IsKeyword(Token const& token) -> bool;
