#pragma once

#include <token.h>

#include <vector>

class Evaluator;

class Stmt
{
public:

	virtual auto Evaluate(Evaluator& evaluator) const -> void = 0;
	virtual ~Stmt() = default;

};

class CompoundStmt : public Stmt
{
private:

	std::vector<std::shared_ptr<Stmt>> m_stmts;

public:

	CompoundStmt(std::vector<std::shared_ptr<Stmt>>&& stmts);

	auto Evaluate(Evaluator& evaluator) const -> void override;

	auto GetStmts() const -> std::vector<std::shared_ptr<Stmt>> const&;

};

class InsertStmt : public Stmt
{
private:

	Token m_key;
	Token m_value;

public:

	InsertStmt(Token const& key, Token const& value);

	auto Evaluate(Evaluator& evaluator) const -> void override;

	auto GetKey() const -> Token const&;
	auto GetValue() const -> Token const&;

};

class SelectStmt : public Stmt
{
private:

	Token m_key;

public:

	SelectStmt(Token const& key);

	auto Evaluate(Evaluator& evaluator) const -> void override;

	auto GetKey() const -> Token const&;

};

class UpdateStmt : public Stmt
{
private:

	Token m_key;
	Token m_value;

public:

	UpdateStmt(Token const& key, Token const& value);

	auto Evaluate(Evaluator& evaluator) const -> void override;

	auto GetKey() const -> Token const&;
	auto GetValue() const -> Token const&;

};

class DeleteStmt : public Stmt
{
private:

	Token m_key;

public:

	DeleteStmt(Token const& key);

	auto Evaluate(Evaluator& evaluator) const -> void override;

	auto GetKey() const -> Token const&;

};
