#pragma once

#include <string>
#include <string_view>

using TokenType_t = std::string;

struct TokenTypes {
  static const std::string_view ILLEGAL;
  static const std::string_view _EOF;

  // Identifiers and Literals
  static const std::string_view IDENT;
  static const std::string_view INT;

  // Operators
  static const std::string_view ASSIGN;
  static const std::string_view PLUS;

  // Delimiters
  static const std::string_view COMMA;
  static const std::string_view SEMICOLON;

  static const std::string_view LPAREN;
  static const std::string_view RPAREN;
  static const std::string_view LBRACE;
  static const std::string_view RBRACE;

  // Keywords
  static const std::string_view FUNCTION;
  static const std::string_view LET;
};

struct Token {
  TokenType_t Type;
  std::string Literal;
};
