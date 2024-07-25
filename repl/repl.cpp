#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
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
    Parser p{&l};
    auto program = p.parseProgram();
    // Print the parsed program

    output << "After Parser: " << program->String() << "\n";

    if (p.getErrors().size() != 0) {
      for (auto &&error : p.getErrors()) {
        output << "\t" << error << "\n";
      }
      continue;
    }
  }
}
