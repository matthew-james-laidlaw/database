#include <database.h>
#include <logger.h>
#include <parser2.h>
#include <token.h>
#include <statement.h>

#include <span>
#include <vector>

auto Parse(std::span<Token>& tokens) -> std::shared_ptr<Stmt>
{
	return ParseCompoundStmt(tokens);
}

auto ParseStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>
{
	auto type = tokens[0].GetType();
	if (type == Token::Type::INSERT)
	{
		return ParseInsertStmt(tokens);
	}
	else if (type == Token::Type::SELECT)
	{
		return ParseSelectStmt(tokens);
	}
	else if (type == Token::Type::UPDATE)
	{
		return ParseUpdateStmt(tokens);
	}
	else if (type == Token::Type::TOK_DELETE)
	{
		return ParseDeleteStmt(tokens);
	}
	else
	{
		g_logger.Error("unexpected token '{}':, expected a command", tokens[0].GetLexeme());
		return nullptr;
	}
}

auto ParseInsertStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>
{
	tokens = tokens.subspan(1);
	auto key = ParseLiteral(tokens);
	auto value = ParseLiteral(tokens);
	return std::make_shared<InsertStmt>(key, value);
}

auto ParseSelectStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>
{
	tokens = tokens.subspan(1);
	auto key = ParseLiteral(tokens);
	return std::make_shared<SelectStmt>(key);
}

auto ParseUpdateStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>
{
	tokens = tokens.subspan(1);
	auto key = ParseLiteral(tokens);
	auto value = ParseLiteral(tokens);
	return std::make_shared<UpdateStmt>(key, value);
}

auto ParseDeleteStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>
{
	tokens = tokens.subspan(1);
	auto key = ParseLiteral(tokens);
	return std::make_shared<DeleteStmt>(key);
}

auto ParseLiteral(std::span<Token>& tokens) -> Token
{
	auto literal = tokens.front();
	tokens = tokens.subspan(1);
	return literal;
}

auto ParseCompoundStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>
{
	auto stmts = std::vector<std::shared_ptr<Stmt>>();
	while (!tokens.empty() && IsKeyword(tokens.front()))
	{
		stmts.push_back(ParseStmt(tokens));
	}
	return std::make_shared<CompoundStmt>(std::move(stmts));
}
