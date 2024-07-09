#include "../lexer/lexer.hpp"
#include "../token/token.hpp"
#include <cstdio>
#include <istream>
#include <ostream>
#include <string>

const std::string PROMPT = ">> ";

void Start(std::istream &input, std::ostream &output) {
  std::string line;
  while (true) {
    output << PROMPT;
    if (!std::getline(input, line)) {
      return;
    }
    Lexer l{line};
    for (Token token{l.nextToken()}; token.Type != TokenTypes::EOF_;
         token = l.nextToken()) {
      output << "Token Type: " << token.Type << ", Literal: " << token.Literal
             << "\n";
    }
  }
}
