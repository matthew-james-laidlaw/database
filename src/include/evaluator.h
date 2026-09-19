#pragma once

#include <database.h>

#include <memory>

class CompoundStmt;
class InsertStmt;
class SelectStmt;
class UpdateStmt;
class DeleteStmt;

class Evaluator
{
private:

	std::shared_ptr<Database> m_db;

public:

	Evaluator(std::shared_ptr<Database> db);

	auto Evaluate(CompoundStmt const& stmt) -> void;
	auto Evaluate(InsertStmt const& stmt) -> void;
	auto Evaluate(SelectStmt const& stmt) -> void;
	auto Evaluate(UpdateStmt const& stmt) -> void;
	auto Evaluate(DeleteStmt const& stmt) -> void;

};
