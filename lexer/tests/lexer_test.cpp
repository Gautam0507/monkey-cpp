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
  std::string input{"let five = 5;"
                    "let ten = 10;"
                    "let add = fn(x, y) { "
                    "   x + y; "
                    "};"
                    "let result = add(five, ten);"
                    "!-/*5;"
                    "5 < 10 > 5;"
                    "if (5 < 10) {"
                    "   return true;"
                    "} else {"
                    "   return false;"
                    "}"
                    "10 == 10;"
                    "10 != 9;"};

  std::vector<TestToken> TestCases = {
      {TokenTypes::LET, "let"},     {TokenTypes::IDENT, "five"},
      {TokenTypes::ASSIGN, "="},    {TokenTypes::INT, "5"},
      {TokenTypes::SEMICOLON, ";"}, {TokenTypes::LET, "let"},
      {TokenTypes::IDENT, "ten"},   {TokenTypes::ASSIGN, "="},
      {TokenTypes::INT, "10"},      {TokenTypes::SEMICOLON, ";"},
      {TokenTypes::LET, "let"},     {TokenTypes::IDENT, "add"},
      {TokenTypes::ASSIGN, "="},    {TokenTypes::FUNCTION, "fn"},
      {TokenTypes::LPAREN, "("},    {TokenTypes::IDENT, "x"},
      {TokenTypes::COMMA, ","},     {TokenTypes::IDENT, "y"},
      {TokenTypes::RPAREN, ")"},    {TokenTypes::LBRACE, "{"},
      {TokenTypes::IDENT, "x"},     {TokenTypes::PLUS, "+"},
      {TokenTypes::IDENT, "y"},     {TokenTypes::SEMICOLON, ";"},
      {TokenTypes::RBRACE, "}"},    {TokenTypes::SEMICOLON, ";"},
      {TokenTypes::LET, "let"},     {TokenTypes::IDENT, "result"},
      {TokenTypes::ASSIGN, "="},    {TokenTypes::IDENT, "add"},
      {TokenTypes::LPAREN, "("},    {TokenTypes::IDENT, "five"},
      {TokenTypes::COMMA, ","},     {TokenTypes::IDENT, "ten"},
      {TokenTypes::RPAREN, ")"},    {TokenTypes::SEMICOLON, ";"},
      {TokenTypes::BANG, "!"},      {TokenTypes::MINUS, "-"},
      {TokenTypes::SLASH, "/"},     {TokenTypes::ASTERISK, "*"},
      {TokenTypes::INT, "5"},       {TokenTypes::SEMICOLON, ";"},
      {TokenTypes::INT, "5"},       {TokenTypes::LT, "<"},
      {TokenTypes::INT, "10"},      {TokenTypes::GT, ">"},
      {TokenTypes::INT, "5"},       {TokenTypes::SEMICOLON, ";"},
      {TokenTypes::IF, "if"},       {TokenTypes::LPAREN, "("},
      {TokenTypes::INT, "5"},       {TokenTypes::LT, "<"},
      {TokenTypes::INT, "10"},      {TokenTypes::RPAREN, ")"},
      {TokenTypes::LBRACE, "{"},    {TokenTypes::RETURN, "return"},
      {TokenTypes::TRUE, "true"},   {TokenTypes::SEMICOLON, ";"},
      {TokenTypes::RBRACE, "}"},    {TokenTypes::ELSE, "else"},
      {TokenTypes::LBRACE, "{"},    {TokenTypes::RETURN, "return"},
      {TokenTypes::FALSE, "false"}, {TokenTypes::SEMICOLON, ";"},
      {TokenTypes::RBRACE, "}"},    {TokenTypes::INT, "10"},
      {TokenTypes::EQ, "=="},       {TokenTypes::INT, "10"},
      {TokenTypes::SEMICOLON, ";"}, {TokenTypes::INT, "10"},
      {TokenTypes::NOT_EQ, "!="},   {TokenTypes::INT, "9"},
      {TokenTypes::SEMICOLON, ";"}, {TokenTypes::EOF_, ""}};

  Lexer l{input};

  for (int i{0}; i < TestCases.size(); i++) {
    Token token = l.nextToken();
    TestToken testToken = TestCases[i];

    EXPECT_EQ(token.Type, testToken.expectedType);
    EXPECT_EQ(token.Literal, testToken.expectedLiteral);
  }
}
