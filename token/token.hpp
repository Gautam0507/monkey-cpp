#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

using TokenType_t = std::string;

struct TokenTypes {
  static const std::string_view ILLEGAL;
  static const std::string_view EOF_;

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

  // Map that stores keywords i.e. builtin identifiers.
  static std::unordered_map<std::string, TokenType_t> keywords;

  Token() : Type{}, Literal{} {};
  Token(std::string_view t, char l) : Type{t}, Literal{std::string{l}} {};

  // Checks if an identifier is a keyword sets the Token.Type value
  void setIdentifier(std::string ident);
};

// Predefined Identifiers
