#include "ast.hpp"
#include "../token/token.hpp"
#include <string>

std::string Program::TokenLiteral() {
  if (statements.size() > 0) {
    return statements[0]->TokenLiteral();
  } else {
    return "";
  }
}
std::string Program::String() {
  std::string info{};
  for (auto &&statement : statements) {
    info += statement->String();
  }
  return info;
}

void Statement::statementNode() {}
std::string Statement::TokenLiteral() { return ""; }

void Expression::expressionNode() {}
std::string Expression::TokenLiteral() { return ""; }

LetStatement::LetStatement(Token &token) : token{token} {};
std::string LetStatement::TokenLiteral() { return token.Literal; }
void LetStatement::statementNode() {}
std::string LetStatement::String() {
  std::string info{};
  info += token.Literal + " ";
  info += name->value + " = ";
  if (value != nullptr) {
    info += value->String();
  }
  info += ";";
  return info;
}

Identifier::Identifier(const Token &t, std::string &s) : token{t}, value{s} {};
std::string Identifier::TokenLiteral() { return token.Literal; }
void Identifier::expressionNode() {}
std::string Identifier::String() { return value; }

ReturnStatement::ReturnStatement(Token &t) : token{t} {}
void ReturnStatement::statementNode() {}
std::string ReturnStatement::TokenLiteral() { return token.Literal; }
std::string ReturnStatement::String() {
  std::string info{};
  info += token.Literal + " ";

  if (returnValue != nullptr) {
    info += returnValue->String();
  }

  info += ";";

  return info;
}

ExpressionStatement::ExpressionStatement(Token &t) : token{t} {}
ExpressionStatement::ExpressionStatement(Token &t,
                                         std::unique_ptr<Expression> e)
    : token{t}, expression{std::move(e)} {}
void ExpressionStatement::statementNode() {}
std::string ExpressionStatement::TokenLiteral() { return token.Literal; }
std::string ExpressionStatement::String() {
  if (expression != nullptr) {
    return expression->String();
  }
  return "";
}

IntegerLiteral::IntegerLiteral(Token &t, int v) : token{t}, value{v} {}
void IntegerLiteral::expressionNode() {}
std::string IntegerLiteral::TokenLiteral() { return token.Literal; }
std::string IntegerLiteral::String() { return std::to_string(value); }

PrefixExpression::PrefixExpression(Token &token, std::string operator_,
                                   std::unique_ptr<Expression> right)
    : token{token}, operator_{operator_}, right{std::move(right)} {}
void PrefixExpression::expressionNode() {}
std::string PrefixExpression::TokenLiteral() { return token.Literal; }
std::string PrefixExpression::String() {
  std::string info = "(" + operator_ + right->String() + ")";
  return info;
}

InfixExpression::InfixExpression(Token &token, std::unique_ptr<Expression> left,
                                 std::string operator_,
                                 std::unique_ptr<Expression> right)
    : token{token}, left{std::move(left)}, operator_{operator_},
      right{std::move(right)} {}
void InfixExpression::expressionNode() {}
std::string InfixExpression::TokenLiteral() { return token.Literal; }
std::string InfixExpression::String() {
  std::string info =
      "(" + left->String() + " " + operator_ + " " + right->String() + ")";
  return info;
}

Boolean::Boolean(Token &token, bool value) : token{token}, value{value} {}
void Boolean::expressionNode() {}
std::string Boolean::TokenLiteral() { return token.Literal; }
std::string Boolean::String() { return token.Literal; }

IfExpression::IfExpression(Token &token) : token{token} {}
IfExpression::IfExpression(Token &token, std::unique_ptr<Expression> condition,
                           std::unique_ptr<BlockStatement> consequence)
    : token{token}, condition{std::move(condition)},
      consequence{std::move(consequence)} {}
IfExpression::IfExpression(Token &token, std::unique_ptr<Expression> condition,
                           std::unique_ptr<BlockStatement> consequence,
                           std::unique_ptr<BlockStatement> alternative)
    : token{token}, condition{std::move(condition)},
      consequence{std::move(consequence)}, alternative{std::move(alternative)} {
}

void IfExpression::expressionNode() {}
std::string IfExpression::TokenLiteral() { return token.Literal; }
std::string IfExpression::String() {
  std::string info = "if" + condition->String() + " " + consequence->String();
  if (alternative != nullptr) {
    info += "else " + alternative->String();
  }
  return info;
}

// Block Statment
BlockStatement::BlockStatement(Token &token) : token{token} {}
BlockStatement::BlockStatement(
    Token &token, std::vector<std::unique_ptr<Statement>> &statements)
    : token{token}, statements{std::move(statements)} {}
void BlockStatement::statementNode() {}
std::string BlockStatement::TokenLiteral() { return token.Literal; }
std::string BlockStatement::String() {
  std::string info{};
  for (auto &&statement : statements) {
    info += statement->String();
  }
  return info;
}

FunctionLiteral::FunctionLiteral(Token &token)
    : token{token}, parameters{}, body{nullptr} {}
void FunctionLiteral::expressionNode() {}
std::string FunctionLiteral::TokenLiteral() { return token.Literal; }
std::string FunctionLiteral::String() {
  std::string info = TokenLiteral() + "(";

  int i = 0;
  for (; i < parameters.size() - 1; ++i) {
    info += parameters[i]->String() + ", ";
  }

  info += parameters[i]->String() + ")";
  info += "{";
  if (body != nullptr) {
    info += body->String();
  }
  info += "}";

  return info;
}
callExpression::callExpression(Token &token)
    : token{token}, function{nullptr} {}
callExpression::callExpression(Token &token,
                               std::unique_ptr<Expression> function)
    : token{token}, function{std::move(function)}, arguments{} {}
void callExpression::expressionNode() {}
std::string callExpression::TokenLiteral() { return token.Literal; }
std::string callExpression::String() {
  std::string info{};
  info += function->String();
  info += "(";
  int i = 0;
  for (; i < arguments.size() - 1; ++i) {
    info += arguments[i]->String() + ", ";
  }
  info += arguments[i]->String() + ")";
  return info;
}
