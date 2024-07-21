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

TEST(Parser, ErrorTest) {
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
  std::vector<std::string> errors = p.getErrors();
  EXPECT_EQ(errors.size(), 0);
  /*TODO:*/
  /*  Make sure to finish the test to be able to print parser errors.*/
}

TEST(Parser, TestReturnStatements) {
  std::string input{"return 5;"
                    "return 10;"
                    "return 993322;"};

  Lexer l{input};
  Parser p{&l};

  std::unique_ptr<Program> program = p.parseProgram();
  EXPECT_NE(program, nullptr);
  EXPECT_EQ(program->statements.size(), 3);

  for (int i{0}; i < 3; i++) {
    ReturnStatement *stmt =
        dynamic_cast<ReturnStatement *>(program->statements[i].get());
    EXPECT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->TokenLiteral(), "return");
  }
}

TEST(Parser, TestIdentifierExpression) {
  std::string input{"foobar;"};

  Lexer l{input};
  Parser p{&l};

  std::unique_ptr<Program> program = p.parseProgram();
  EXPECT_NE(program, nullptr);
  EXPECT_EQ(program->statements.size(), 1);

  ExpressionStatement *stmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  EXPECT_NE(stmt, nullptr);

  Identifier *ident = dynamic_cast<Identifier *>(stmt->expression.get());
  EXPECT_NE(ident, nullptr);
  EXPECT_EQ(ident->value, "foobar");
  EXPECT_EQ(ident->TokenLiteral(), "foobar");
}

TEST(Parser, TestIntegerLiteralExpresssion) {
  std::string input{"5;"};

  Lexer l{input};
  Parser p{&l};

  std::unique_ptr<Program> program = p.parseProgram();
  EXPECT_NE(program, nullptr);
  EXPECT_EQ(program->statements.size(), 1);

  ExpressionStatement *stmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  EXPECT_NE(stmt, nullptr);

  IntegerLiteral *intLit =
      dynamic_cast<IntegerLiteral *>(stmt->expression.get());
  EXPECT_NE(intLit, nullptr);
  EXPECT_EQ(intLit->value, 5);
  EXPECT_EQ(intLit->TokenLiteral(), "5");
}
