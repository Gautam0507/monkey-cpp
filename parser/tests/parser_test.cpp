#include "../../lexer/lexer.hpp"
#include "../parser.hpp"
#include "gtest/gtest.h"
#include <memory>
#include <string>
#include <vector>
struct testIdentifier {
  std::string ExpectedIdentifier;

  testIdentifier(std::string expectedIdentifier)
      : ExpectedIdentifier(expectedIdentifier) {}
};

TEST(Parser, LetStatementTest) {
  std::string input{"let x = 5;"
                    "let y = 10;"
                    "let foobar = 838383;"};
  Lexer l{input};
  Parser p{&l};

  std::vector<testIdentifier> tests = {
      {"x"},
      {"y"},
      {"foobar"},
  };

  std::unique_ptr<Program> program = p.parseProgram();
  ASSERT_NE(program, nullptr);
  ASSERT_EQ(program->statements.size(), 3);

  for (int i = 0; i < tests.size(); i++) {
    LetStatement *stmt =
        dynamic_cast<LetStatement *>(program->statements[i].get());
    ASSERT_NE(stmt, nullptr);
    ASSERT_EQ(stmt->TokenLiteral(), "let");
    ASSERT_EQ(stmt->name->value, tests[i].ExpectedIdentifier);
    ASSERT_EQ(stmt->name->TokenLiteral(), tests[i].ExpectedIdentifier);
  }
}
