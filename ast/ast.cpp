#include "ast.hpp"
#include "token.hpp"
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
