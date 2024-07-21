#pragma once
#include "../ast/ast.hpp"
#include "../lexer/lexer.hpp"
#include "../token/token.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <vector>
class Parser {
private:
  Lexer *lexer;
  Token CurrentToken;
  Token peekToken;
  std::vector<std::string> errors;

public:
  Parser() = delete;
  Parser(Lexer *l);

  void nextToken();
  std::unique_ptr<Program> parseProgram();
  std::unique_ptr<Statement> parseStatement();
  std::unique_ptr<LetStatement> parseLetStatement();
  std::unique_ptr<ReturnStatement> parseReturnStatement();
  std::vector<std::string> &getErrors();
  void PeekError(std::string_view &t);
  bool curTokenIs(std::string_view &t);
  bool peekTokenIs(std::string_view &t);
  bool expectPeek(std::string_view &t);
};
