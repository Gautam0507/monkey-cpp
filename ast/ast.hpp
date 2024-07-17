#pragma once
#include "token.hpp"
#include <memory>
#include <string>
#include <vector>

class Node {
public:
  virtual std::string TokenLiteral() = 0;
  virtual ~Node() = default;
};

class Statement : public Node {
public:
  std::string TokenLiteral() override;
  virtual void statementNode() = 0;
};

class Expression : public Node {
public:
  std::string TokenLiteral() override;
  virtual void expressionNode() = 0;
};

class Program : public Node {
public:
  std::vector<std::unique_ptr<Statement>> statements{};
  std::string TokenLiteral() override;
};

class Identifier : public Expression {
public:
  Identifier(const Token &, std::string &);
  Token token;
  std::string value;

  void expressionNode() override;
  std::string TokenLiteral() override;
};

class LetStatement : public Statement {
public:
  Token token;
  LetStatement(Token &);
  std::unique_ptr<Identifier> name;
  std::unique_ptr<Expression> value;

  void statementNode() override;
  std::string TokenLiteral() override;
};

class ReturnStatement : public Statement {
public:
  Token token;
  std::unique_ptr<Expression> returnValue;

  void statementNode() override;
  std::string TokenLiteral() override;
};
