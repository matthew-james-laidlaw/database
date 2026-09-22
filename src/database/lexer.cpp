#include <lexer.h>

auto Lex(std::string_view& source) -> std::vector<Token>
{
	auto tokens = std::vector<Token>();

	auto insert_cmd = std::string("INSERT");
	auto select_cmd = std::string("SELECT");
	auto update_cmd = std::string("UPDATE");
	auto delete_cmd = std::string("DELETE");

	while (!source.empty())
	{
		auto lexeme = std::string(source.substr(0, source.find_first_of(' ')));
		if (lexeme == insert_cmd)
		{
			source = source.substr(insert_cmd.size());
			tokens.emplace_back("INSERT", Token::Type::INSERT);
		}
		else if (lexeme == select_cmd)
		{
			source = source.substr(select_cmd.size());
			tokens.emplace_back("SELECT", Token::Type::SELECT);
		}
		else if (lexeme == update_cmd)
		{
			source = source.substr(update_cmd.size());
			tokens.emplace_back("UPDATE", Token::Type::UPDATE);
		}
		else if (lexeme == delete_cmd)
		{
			source = source.substr(delete_cmd.size());
			tokens.emplace_back("DELETE", Token::Type::TOK_DELETE);
		}
		else
		{
			source = source.substr(source.find_first_not_of(' '));
			tokens.emplace_back(lexeme, Token::Type::LITERAL);
		}

		// trim whitespace up to the next token
		while (!source.empty() && std::isspace(source.front()))
		{
			source = source.substr(1);
		}
	}

	return tokens;
}
