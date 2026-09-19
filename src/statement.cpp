#include <evaluator.h>
#include <statement.h>

CompoundStmt::CompoundStmt(std::vector<std::shared_ptr<Stmt>>&& stmts)
	: m_stmts(std::move(stmts))
{}

auto CompoundStmt::Evaluate(Evaluator& evaluator) const -> void
{
	return evaluator.Evaluate(*this);
}

auto CompoundStmt::GetStmts() const -> std::vector<std::shared_ptr<Stmt>> const&
{
	return m_stmts;
}

InsertStmt::InsertStmt(Token const& key, Token const& value)
	: m_key(key), m_value(value)
{}

auto InsertStmt::Evaluate(Evaluator& evaluator) const -> void
{
	return evaluator.Evaluate(*this);
}

auto InsertStmt::GetKey() const -> Token const&
{
	return m_key;
}

auto InsertStmt::GetValue() const -> Token const&
{
	return m_value;
}

SelectStmt::SelectStmt(Token const& key)
	: m_key(key)
{}

auto SelectStmt::Evaluate(Evaluator& evaluator) const -> void
{
	return evaluator.Evaluate(*this);
}

auto SelectStmt::GetKey() const -> Token const&
{
	return m_key;
}

UpdateStmt::UpdateStmt(Token const& key, Token const& value)
	: m_key(key), m_value(value)
{}

auto UpdateStmt::Evaluate(Evaluator& evaluator) const -> void
{
	return evaluator.Evaluate(*this);
}

auto UpdateStmt::GetKey() const -> Token const&
{
	return m_key;
}

auto UpdateStmt::GetValue() const -> Token const&
{
	return m_value;
}

DeleteStmt::DeleteStmt(Token const& key)
	: m_key(key)
{}

auto DeleteStmt::Evaluate(Evaluator& evaluator) const -> void
{
	return evaluator.Evaluate(*this);
}

auto DeleteStmt::GetKey() const -> Token const&
{
	return m_key;
}
