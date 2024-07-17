#pragma once
#include "../ast/ast.hpp"
#include "../lexer/lexer.hpp"
#include "../token/token.hpp"
#include <memory>
#include <string_view>
class Parser {
private:
  Lexer *lexer;
  Token CurrentToken;
  Token peekToken;

public:
  Parser() = delete;
  Parser(Lexer *l);

  void nextToken();
  std::unique_ptr<Program> parseProgram();
  std::unique_ptr<Statement> parseStatement();
  std::unique_ptr<LetStatement> parseLetStatement();
  bool curTokenIs(const std::string_view &t);
  bool peekTokenIs(const std::string_view &t);
  bool expectPeek(const std::string_view &t);
};
