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

void Statement::statementNode() {}
std::string Statement::TokenLiteral() { return ""; }

void Expression::expressionNode() {}
std::string Expression::TokenLiteral() { return ""; }

LetStatement::LetStatement(Token &token) : token{token} {};
std::string LetStatement::TokenLiteral() { return token.Literal; }
void LetStatement::statementNode() {}

Identifier::Identifier(const Token &t, std::string &s) : token{t}, value{s} {};
std::string Identifier::TokenLiteral() { return token.Literal; }
void Identifier::expressionNode() {}
