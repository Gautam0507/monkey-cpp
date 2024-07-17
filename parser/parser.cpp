#include "parser.hpp"
#include "token.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../lexer/lexer.hpp"
#include "../token/token.hpp"

Parser::Parser(Lexer *l) : lexer(l) {
  // Read 2 tokens
  nextToken();
  nextToken();
}

void Parser::nextToken() {
  CurrentToken = peekToken;
  peekToken = lexer->nextToken();
}

std::unique_ptr<LetStatement> Parser::parseLetStatement() {
  std::unique_ptr<LetStatement> stmt =
      std::make_unique<LetStatement>(CurrentToken);

  if (!expectPeek(TokenTypes::IDENT)) {
    return nullptr;
  }

  stmt->name = std::make_unique<Identifier>(CurrentToken, CurrentToken.Literal);

  if (!expectPeek(TokenTypes::ASSIGN)) {
    return nullptr;
  }

  // TODO: We're skipping the expressions until we encounter a semicolon

  while (!curTokenIs(TokenTypes::SEMICOLON)) {
    nextToken();
  }
  return std::move(stmt);
}

std::unique_ptr<Statement> Parser::parseStatement() {
  if (CurrentToken.Type == TokenTypes::LET) {
    return std::move(parseLetStatement());
  } else {
    return nullptr;
  }
}

std::unique_ptr<Program> Parser::parseProgram() {
  std::unique_ptr<Program> program = std::make_unique<Program>();
  program->statements = std::vector<std::unique_ptr<Statement>>{};

  while (!curTokenIs(TokenTypes::EOF_)) {
    std::unique_ptr<Statement> stmt = parseStatement();
    if (stmt != nullptr) {
      program->statements.push_back(std::move(stmt));
    }
    nextToken();
  }
  return std::move(program);
}

bool Parser::curTokenIs(const std::string_view &type) {
  return CurrentToken.Type == type;
}

bool Parser::peekTokenIs(const std::string_view &type) {
  return peekToken.Type == type;
}

bool Parser::expectPeek(const std::string_view &type) {
  if (peekTokenIs(type)) {
    nextToken();
    return true;
  } else {
    return false;
  }
}
