#pragma once
#include "../token/token.hpp"
#include <string>

class Lexer {

private:
  std::string input;
  int position;
  int readPosition;
  char ch;

public:
  Lexer(std::string in) : input{in}, position{}, readPosition{}, ch{} {
    readChar();
  };

  // Advance the position and give us the next charachter
  void readChar();

  // Reads the ch and returns the corresponding token
  Token nextToken();

  // Reads the current identifier and returns the identifier as a string.
  std::string readIdentifier();

  // The function defines all the allowed charachters in an identifier.
  bool isLetter(char ch);
};
