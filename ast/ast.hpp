#pragma once
#include "token.hpp"
#include <memory>
#include <string>
#include <vector>

class Node {
public:
  virtual std::string TokenLiteral() = 0;
  virtual std::string String() = 0;
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
  std::string String() override;
};

class Identifier : public Expression {
public:
  Identifier(const Token &, std::string &);
  Token token;
  std::string value;

  void expressionNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class LetStatement : public Statement {
public:
  Token token;
  LetStatement(Token &);
  std::unique_ptr<Identifier> name;
  std::unique_ptr<Expression> value;

  void statementNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class ReturnStatement : public Statement {
public:
  Token token;
  ReturnStatement(Token &);
  std::unique_ptr<Expression> returnValue;

  void statementNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class ExpressionStatement : public Statement {
public:
  Token token;
  std::unique_ptr<Expression> expression;
  ExpressionStatement(Token &);

  void statementNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class IntegerLiteral : public Expression {
public:
  Token token;
  int value;
  IntegerLiteral(Token &, int);

  void expressionNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class PrefixExpression : public Expression {
public:
  Token token;
  std::string operator_;
  std::unique_ptr<Expression> right;
  PrefixExpression(Token &, std::string, std::unique_ptr<Expression>);

  void expressionNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class InfixExpression : public Expression {
public:
  Token token;
  std::unique_ptr<Expression> left;
  std::string operator_;
  std::unique_ptr<Expression> right;
  InfixExpression(Token &, std::unique_ptr<Expression>, std::string,
                  std::unique_ptr<Expression>);

  void expressionNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class Boolean : public Expression {
public:
  Token token;
  bool value;

  Boolean(Token &, bool);
  void expressionNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class BlockStatement : public Statement {
public:
  Token token;
  std::vector<std::unique_ptr<Statement>> statements;

  BlockStatement(Token &);
  BlockStatement(Token &, std::vector<std::unique_ptr<Statement>> &);
  void statementNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class IfExpression : public Expression {
public:
  Token token;
  std::unique_ptr<Expression> condition;
  std::unique_ptr<BlockStatement> consequence;
  std::unique_ptr<BlockStatement> alternative;

  IfExpression(Token &);
  IfExpression(Token &, std::unique_ptr<Expression>,
               std::unique_ptr<BlockStatement>);
  IfExpression(Token &, std::unique_ptr<Expression>,
               std::unique_ptr<BlockStatement>,
               std::unique_ptr<BlockStatement>);
  void expressionNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};

class FunctionLiteral : public Expression {
public:
  Token token;
  std::vector<std::unique_ptr<Identifier>> parameters;
  std::unique_ptr<BlockStatement> body;

  FunctionLiteral(Token &);
  void expressionNode() override;
  std::string TokenLiteral() override;
  std::string String() override;
};
