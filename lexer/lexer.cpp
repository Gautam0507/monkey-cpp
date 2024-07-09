#include "lexer.hpp"
#include "../token/token.hpp"
#include <cctype>
#include <cstdio>
#include <string>

void Lexer::readChar() {
  if (readPosition >= input.size()) {
    ch = 0;
  } else {
    ch = input[readPosition];
  }
  position = readPosition;
  readPosition++;
};

Token Lexer::nextToken() {
  Token token{};

  skipWhitespace();

  switch (ch) {
  case ';':
    token = Token(TokenTypes::SEMICOLON, ch);
    break;
  case '(':
    token = Token(TokenTypes::LPAREN, ch);
    break;
  case ')':
    token = Token(TokenTypes::RPAREN, ch);
    break;
  case ',':
    token = Token(TokenTypes::COMMA, ch);
    break;
  case '+':
    token = Token(TokenTypes::PLUS, ch);
    break;
  case '{':
    token = Token(TokenTypes::LBRACE, ch);
    break;
  case '}':
    token = Token(TokenTypes::RBRACE, ch);
    break;
  case '-':
    token = Token(TokenTypes::MINUS, ch);
    break;
  case '*':
    token = Token(TokenTypes::ASTERISK, ch);
    break;
  case '/':
    token = Token(TokenTypes::SLASH, ch);
    break;
  case '<':
    token = Token(TokenTypes::LT, ch);
    break;
  case '>':
    token = Token(TokenTypes::GT, ch);
    break;
  case '=':
    if (peekChar() == '=') {
      readChar();
      token.Type = TokenTypes::EQ;
      token.Literal = "==";
    } else {
      token = Token(TokenTypes::ASSIGN, ch);
    }
    break;
  case '!':
    if (peekChar() == '=') {
      readChar();
      token.Type = TokenTypes::NOT_EQ;
      token.Literal = "!=";
    } else {
      token = Token(TokenTypes::BANG, ch);
    }
    break;
  case 0:
    token.Type = TokenTypes::EOF_;
    token.Literal = "";
    break;
  default:
    if (isLetter(ch)) {
      token.Literal = readIdentifier();
      token.setIdentifier(token.Literal);
      return token;
    } else if (isDigit(ch)) {
      token.Type = TokenTypes::INT;
      token.Literal = readNumber();
      return token;
    } else {

      token = Token(TokenTypes::ILLEGAL, ch);
    }
  }
  readChar();
  return token;
}

std::string Lexer::readIdentifier() {
  int pos{position};
  while (isLetter(ch)) {
    readChar();
  }
  std::string identifier = input.substr(pos, position - pos);
  return identifier;
}

void Lexer::skipWhitespace() {
  while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
    readChar();
  }
}

std::string Lexer::readNumber() {
  int pos{position};
  while (isDigit(ch)) {
    readChar();
  }
  return input.substr(pos, position - pos);
}

char Lexer::peekChar() {
  if (readPosition >= input.size()) {
    return 0;
  } else {
    return input[readPosition];
  }
}

bool isLetter(char ch) {
  return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
}

bool isDigit(char ch) { return ('0' <= ch && ch <= '9'); }
