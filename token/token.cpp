#include "token.hpp"
#include <string>
#include <string_view>
#include <unordered_map>

const std::string_view TokenTypes::ILLEGAL{"ILLEGAL"};
const std::string_view TokenTypes::EOF_{"EOF"};

// Identifiers and Literals
const std::string_view TokenTypes::IDENT{"IDENT"};
const std::string_view TokenTypes::INT{"INT"};

// Operators
const std::string_view TokenTypes::ASSIGN{"="};
const std::string_view TokenTypes::PLUS{"+"};

// Delimiters
const std::string_view TokenTypes::COMMA{","};
const std::string_view TokenTypes::SEMICOLON{";"};

const std::string_view TokenTypes::LPAREN{"("};
const std::string_view TokenTypes::RPAREN{")"};
const std::string_view TokenTypes::LBRACE{"{"};
const std::string_view TokenTypes::RBRACE{"}"};

// Keywords
const std::string_view TokenTypes::FUNCTION{"FUNCTION"};
const std::string_view TokenTypes::LET{"LET"};

std::unordered_map<std::string, TokenType_t> Token::keywords{
    {"fn", std::string(TokenTypes::FUNCTION)},
    {"let", std::string(TokenTypes::LET)}};

void Token::setIdentifier(std::string ident) {
  if (keywords.count(keywords[ident])) {
    Type = keywords[ident];
  } else {
    Type = TokenTypes::IDENT;
  }
}
