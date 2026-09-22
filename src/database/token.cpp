#include <token.h>

Token::Token(std::string lexeme, Type type)
	: m_lexeme(lexeme), m_type(type)
{}

auto Token::GetLexeme() const -> std::string
{
	return m_lexeme;
}

auto Token::GetType() const -> Type
{
	return m_type;
}

auto IsKeyword(Token const& token) -> bool
{
	auto type = token.GetType();
	if (type == Token::Type::INSERT || type == Token::Type::SELECT ||
		type == Token::Type::UPDATE || type == Token::Type::TOK_DELETE)
	{
		return true;
	}
	return false;
}
