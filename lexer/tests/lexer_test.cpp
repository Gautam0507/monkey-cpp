#include "../../token/token.hpp"
#include "../lexer.hpp"
#include "gtest/gtest.h"
#include <string>
#include <string_view>
#include <vector>

struct TestToken {
  TokenType_t expectedType;
  std::string expectedLiteral;

  TestToken(std::string_view t, std::string l)
      : expectedType{t}, expectedLiteral{l} {};
};

TEST(Lexer, TestNextToken) {
  std::string input{"=+(){},;"};

  std::vector<TestToken> TestCases = {
      {TokenTypes::ASSIGN, "="}, {TokenTypes::PLUS, "+"},
      {TokenTypes::LPAREN, "("}, {TokenTypes::RPAREN, ")"},
      {TokenTypes::LBRACE, "{"}, {TokenTypes::RBRACE, "}"},
      {TokenTypes::COMMA, ","},  {TokenTypes::SEMICOLON, ";"},
      {TokenTypes::EOF_, ""},
  };

  Lexer l{input};

  for (int i; i < TestCases.size(); i++) {
    Token token = l.nextToken();
    TestToken testToken = TestCases[i];

    EXPECT_EQ(token.Type, testToken.expectedType);
    EXPECT_EQ(token.Literal, testToken.expectedLiteral);
  }
};
