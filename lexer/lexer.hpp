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

  // Function to skip over whitespace because we do not consider it useful.
  void skipWhitespace();

  // Reads the current number fully and returns the number using the isDigit
  // function

  std::string readNumber();
};

// Helper Functions
// The function defines all the allowed charachters in an identifier.
bool isLetter(char ch);

// Function to check for integers and numbers
bool isDigit(char ch);
