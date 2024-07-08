#pragma once
#include "../token/token.hpp"
#include <string>

class Lexer {
public:
  Lexer(std::string in) : input{in}, position{}, readPosition{}, ch{} {
    readChar();
  };

  // Advance the position and give us the next charachter
  void readChar();

  Token nextToken();

private:
  std::string input;
  int position;
  int readPosition;
  char ch;
};
