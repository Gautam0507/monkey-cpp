#include "parser.hpp"
#include "ast.hpp"
#include "token.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../lexer/lexer.hpp"
#include "../token/token.hpp"

enum class Precedence {
  LOWEST = 0,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIX,
  CALL,
  INDEX,
};

Parser::Parser(Lexer *l)
    : lexer{l}, errors{}, prefixParseFns{}, infixParseFns{} {
  // Read 2 tokens
  nextToken();
  nextToken();

  precedences = {
      {std::string(TokenTypes::EQ), Precedence::EQUALS},
      {std::string(TokenTypes::NOT_EQ), Precedence::EQUALS},
      {std::string(TokenTypes::LT), Precedence::LESSGREATER},
      {std::string(TokenTypes::GT), Precedence::LESSGREATER},
      {std::string(TokenTypes::PLUS), Precedence::SUM},
      {std::string(TokenTypes::MINUS), Precedence::SUM},
      {std::string(TokenTypes::SLASH), Precedence::PRODUCT},
      {std::string(TokenTypes::ASTERISK), Precedence::PRODUCT},
      {std::string(TokenTypes::LPAREN), Precedence::CALL},
      {std::string(TokenTypes::LBRACE), Precedence::INDEX},
  };

  registerPrefix(std::string(TokenTypes::IDENT),
                 std::bind(&Parser::parseIdentifier, this));
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

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement() {
  std::unique_ptr<ReturnStatement> stmt =
      std::make_unique<ReturnStatement>(CurrentToken);

  nextToken();

  stmt->returnValue = std::move(parseExpression(Precedence::LOWEST));

  if (peekTokenIs(TokenTypes::SEMICOLON)) {
    nextToken();
  }

  return std::move(stmt);
}

std::unique_ptr<Statement> Parser::parseStatement() {
  if (CurrentToken.Type == TokenTypes::LET) {
    return std::move(parseLetStatement());
  } else if (CurrentToken.Type == TokenTypes::RETURN) {
    return std::move(parseReturnStatement());
  } else {
    return std::move(parseExpressionStatement());
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

bool Parser::curTokenIs(std::string_view &type) {
  return CurrentToken.Type == type;
}

bool Parser::peekTokenIs(std::string_view &type) {
  return peekToken.Type == type;
}

bool Parser::expectPeek(std::string_view &type) {
  if (peekTokenIs(type)) {
    nextToken();
    return true;
  } else {
    PeekError(type);
    return false;
  }
}

std::vector<std::string> &Parser::getErrors() { return errors; }

void Parser::PeekError(std::string_view &t) {
  std::string msg =
      "Expected next token to be: " + std::string(t) + "got: " + peekToken.Type;
  errors.push_back(msg);
}

void Parser::registerPrefix(TokenType_t tokenType, prefixParseFn fn) {
  prefixParseFns[tokenType] = fn;
}

void Parser::registerInfix(TokenType_t tokenType, infixParseFn fn) {
  infixParseFns[tokenType] = fn;
}

std::unique_ptr<ExpressionStatement> Parser::parseExpressionStatement() {
  std::unique_ptr<ExpressionStatement> stmt =
      std::make_unique<ExpressionStatement>(CurrentToken);
  stmt->expression = std::move(parseExpression(Precedence::LOWEST));
  if (peekTokenIs(TokenTypes::SEMICOLON)) {
    nextToken();
  }
  return std::move(stmt);
}

std::unique_ptr<Expression> Parser::parseExpression(Precedence precedence) {
  if (!prefixParseFns.count(CurrentToken.Type)) {
    return nullptr;
  }
  auto &&prefix = prefixParseFns[CurrentToken.Type];

  std::unique_ptr<Expression> leftExp = prefix();

  while (!peekTokenIs(TokenTypes::SEMICOLON) && precedence < peekPrecedence()) {
    if (!infixParseFns.count(peekToken.Type)) {
      return std::move(leftExp);
    }

    auto &&infix = infixParseFns[peekToken.Type];

    nextToken();

    leftExp = std::move(infix(std::move(leftExp)));
  }
  return std::move(leftExp);
}

Precedence Parser::peekPrecedence() {
  if (precedences.find(peekToken.Type) != precedences.end()) {
    return precedences[peekToken.Type];
  }
  return Precedence::LOWEST;
}

std::unique_ptr<Expression> Parser::parseIdentifier() {
  return std::move(
      std::make_unique<Identifier>(CurrentToken, CurrentToken.Literal));
}
