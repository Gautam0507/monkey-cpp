#include "token.hpp"
#include <string_view>

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
