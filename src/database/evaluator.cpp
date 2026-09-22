#include <evaluator.h>
#include <logger.h>
#include <statement.h>

#include <stdexcept>

Evaluator::Evaluator(std::shared_ptr<Database> db)
	: m_db(db)
{
	if (!db)
	{
		throw std::runtime_error("cannot pass null database to Evaluator::Evaluator()");
	}
}

auto Evaluator::Evaluate(CompoundStmt const& stmt) -> void
{
	for (auto const& item : stmt.GetStmts())
	{
		item->Evaluate(*this);
	}
}

auto Evaluator::Evaluate(InsertStmt const& stmt) -> void
{
	auto inserted = m_db->Insert(stmt.GetKey().GetLexeme(), stmt.GetValue().GetLexeme());
	if (inserted == 0)
	{
		g_logger.Info("inserted 0 item(s): duplicate key: '{}'", stmt.GetKey().GetLexeme());
	}
	else
	{
		g_logger.Info("inserted 1 item(s): key: '{}', value: '{}'", stmt.GetKey().GetLexeme(), stmt.GetValue().GetLexeme());
	}
}

auto Evaluator::Evaluate(SelectStmt const& stmt) -> void
{
	auto selected = m_db->Select(stmt.GetKey().GetLexeme());
	if (selected)
	{
		g_logger.Info("selected 1 item(s): key: '{}', value: '{}'", stmt.GetKey().GetLexeme(), *selected);
	}
	else
	{
		g_logger.Info("selected 0 item(s): nonexistent key: '{}'", stmt.GetKey().GetLexeme());
	}
}

auto Evaluator::Evaluate(UpdateStmt const& stmt) -> void
{
	auto updated = m_db->Update(stmt.GetKey().GetLexeme(), stmt.GetValue().GetLexeme());
	if (updated == 0)
	{
		g_logger.Info("updated 0 item(s): nonexistent key: '{}'", stmt.GetKey().GetLexeme());
	}
	else
	{
		g_logger.Info("updated 1 item(s): key: '{}', value: '{}'", stmt.GetKey().GetLexeme(), stmt.GetValue().GetLexeme());
	}
}

auto Evaluator::Evaluate(DeleteStmt const& stmt) -> void
{
	auto deleted = m_db->Delete(stmt.GetKey().GetLexeme());
	if (deleted == 0)
	{
		g_logger.Info("deleted 0 item(s): nonexistent key: '{}'", stmt.GetKey().GetLexeme());
	}
	else
	{
		g_logger.Info("deleted 1 item(s): key: '{}'", stmt.GetKey().GetLexeme());
	}
}
