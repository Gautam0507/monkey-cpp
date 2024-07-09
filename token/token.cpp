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
const std::string_view TokenTypes::MINUS{"-"};
const std::string_view TokenTypes::BANG{"!"};
const std::string_view TokenTypes::ASTERISK{"*"};
const std::string_view TokenTypes::SLASH{"/"};

const std::string_view TokenTypes::LT{"<"};
const std::string_view TokenTypes::GT{">"};
const std::string_view TokenTypes::EQ{"=="};
const std::string_view TokenTypes::NOT_EQ{"!="};

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
const std::string_view TokenTypes::TRUE{"TRUE"};
const std::string_view TokenTypes::FALSE{"FALSE"};
const std::string_view TokenTypes::IF{"IF"};
const std::string_view TokenTypes::ELSE{"ELSE"};
const std::string_view TokenTypes::RETURN{"RETURN"};

std::unordered_map<std::string, TokenType_t> Token::keywords{
    {"fn", std::string(TokenTypes::FUNCTION)},
    {"let", std::string(TokenTypes::LET)},
    {"fn", std::string(TokenTypes::FUNCTION)},
    {"let", std::string(TokenTypes::LET)},
    {"true", std::string(TokenTypes::TRUE)},
    {"false", std::string(TokenTypes::FALSE)},
    {"if", std::string(TokenTypes::IF)},
    {"else", std::string(TokenTypes::ELSE)},
    {"return", std::string(TokenTypes::RETURN)},

};

void Token::setIdentifier(std::string ident) {
  if (auto search = keywords.find(ident); search != keywords.end()) {
    Type = search->second;
  } else {
    Type = TokenTypes::IDENT;
  }
}
