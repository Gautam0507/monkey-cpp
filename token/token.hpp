#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

using TokenType_t = std::string;

struct TokenTypes {
  static std::string_view ILLEGAL;
  static std::string_view EOF_;

  // Identifiers and Literals
  static std::string_view IDENT;
  static std::string_view INT;

  // Operators
  static std::string_view ASSIGN;
  static std::string_view PLUS;
  static std::string_view MINUS;
  static std::string_view BANG;
  static std::string_view ASTERISK;
  static std::string_view SLASH;

  static std::string_view LT;
  static std::string_view GT;

  static std::string_view EQ;
  static std::string_view NOT_EQ;

  // Delimiters
  static std::string_view COMMA;
  static std::string_view SEMICOLON;

  static std::string_view LPAREN;
  static std::string_view RPAREN;
  static std::string_view LBRACE;
  static std::string_view RBRACE;

  // Keywords
  static std::string_view FUNCTION;
  static std::string_view LET;
  static std::string_view TRUE;
  static std::string_view FALSE;
  static std::string_view IF;
  static std::string_view ELSE;
  static std::string_view RETURN;
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
