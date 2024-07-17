#include "token.hpp"
#include <string>
#include <string_view>
#include <unordered_map>

std::string_view TokenTypes::ILLEGAL{"ILLEGAL"};
std::string_view TokenTypes::EOF_{"EOF"};

// Identifiers and Literals
std::string_view TokenTypes::IDENT{"IDENT"};
std::string_view TokenTypes::INT{"INT"};

// Operators
std::string_view TokenTypes::ASSIGN{"="};
std::string_view TokenTypes::PLUS{"+"};
std::string_view TokenTypes::MINUS{"-"};
std::string_view TokenTypes::BANG{"!"};
std::string_view TokenTypes::ASTERISK{"*"};
std::string_view TokenTypes::SLASH{"/"};

std::string_view TokenTypes::LT{"<"};
std::string_view TokenTypes::GT{">"};
std::string_view TokenTypes::EQ{"=="};
std::string_view TokenTypes::NOT_EQ{"!="};

// Delimiters
std::string_view TokenTypes::COMMA{","};
std::string_view TokenTypes::SEMICOLON{";"};

std::string_view TokenTypes::LPAREN{"("};
std::string_view TokenTypes::RPAREN{")"};
std::string_view TokenTypes::LBRACE{"{"};
std::string_view TokenTypes::RBRACE{"}"};

// Keywords
std::string_view TokenTypes::FUNCTION{"FUNCTION"};
std::string_view TokenTypes::LET{"LET"};
std::string_view TokenTypes::TRUE{"TRUE"};
std::string_view TokenTypes::FALSE{"FALSE"};
std::string_view TokenTypes::IF{"IF"};
std::string_view TokenTypes::ELSE{"ELSE"};
std::string_view TokenTypes::RETURN{"RETURN"};

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
