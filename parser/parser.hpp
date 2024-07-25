#pragma once
#include "../ast/ast.hpp"
#include "../lexer/lexer.hpp"
#include "../token/token.hpp"
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using prefixParseFn = std::function<std::unique_ptr<Expression>()>;
using infixParseFn =
    std::function<std::unique_ptr<Expression>(std::unique_ptr<Expression>)>;

enum class Precedence;

class Parser {
private:
  Lexer *lexer;
  Token CurrentToken;
  Token peekToken;
  std::vector<std::string> errors;
  std::unordered_map<TokenType_t, prefixParseFn> prefixParseFns;
  std::unordered_map<TokenType_t, infixParseFn> infixParseFns;

  std::unordered_map<std::string, Precedence> precedences;

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

  void registerPrefix(TokenType_t tokenType, prefixParseFn fn);
  void registerInfix(TokenType_t tokenType, infixParseFn fn);
  void noPrefixParseFnError(TokenType_t t);

  std::unique_ptr<ExpressionStatement> parseExpressionStatement();
  std::unique_ptr<Expression> parseExpression(Precedence precedence);
  std::unique_ptr<Expression> parseIdentifier();
  std::unique_ptr<Expression> parseIntegerLiteral();
  std::unique_ptr<Expression> parsePrefixExpression();
  std::unique_ptr<Expression> parseBoolean();
  std::unique_ptr<Expression>
  parseInfixExpression(std::unique_ptr<Expression> leftExpr);
  std::unique_ptr<Expression> parseGroupedExpression();
  std::unique_ptr<Expression> parseIfExpression();
  std::unique_ptr<BlockStatement> parseBlockStatement();
  std::unique_ptr<Expression> parseFunctionLiteral();
  std::vector<std::unique_ptr<Identifier>> parseFunctionParameters();
  std::unique_ptr<Expression>
  parseCallExpression(std::unique_ptr<Expression> fn);
  std::vector<std::unique_ptr<Expression>> parseCallArguments();

  Precedence peekPrecedence();
  Precedence curPrecedence();
};
