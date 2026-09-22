#pragma once

#include <token.h>
#include <statement.h>

#include <span>
#include <memory>

auto Parse(std::span<Token>& tokens) -> std::shared_ptr<Stmt>;
auto ParseStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>;
auto ParseCompoundStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>;
auto ParseInsertStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>;
auto ParseSelectStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>;
auto ParseUpdateStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>;
auto ParseDeleteStmt(std::span<Token>& tokens) -> std::shared_ptr<Stmt>;
auto ParseLiteral(std::span<Token>& tokens) -> Token;
