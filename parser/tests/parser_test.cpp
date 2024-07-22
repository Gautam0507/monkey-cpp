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

struct PrefixTests {
  std::string input;
  std::string operator_;
  int value;
};

std::string PrintErrors(std::vector<std::string> &errors) {
  std::string errorString;
  for (auto &&error : errors) {
    errorString += error + "\n";
  }
  return errorString;
}

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
  ASSERT_NE(program, nullptr) << "Program is nullptr";
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
  EXPECT_EQ(errors.size(), 0) << "Parser has errors" << std::endl
                              << PrintErrors(errors);
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

TEST(Parser, TestParsingPrefixExpressions) {
  std::vector<PrefixTests> tests = {
      {"!5;", "!", 5},
      {"-15;", "-", 15},
  };

  for (auto &&test : tests) {
    Lexer l{test.input};
    Parser p{&l};

    std::unique_ptr<Program> program = p.parseProgram();
    EXPECT_NE(program, nullptr);
    EXPECT_EQ(program->statements.size(), 1);

    ExpressionStatement *stmt =
        dynamic_cast<ExpressionStatement *>(program->statements[0].get());
    EXPECT_NE(stmt, nullptr);

    PrefixExpression *prefixExpr =
        dynamic_cast<PrefixExpression *>(stmt->expression.get());
    EXPECT_NE(prefixExpr, nullptr);
    EXPECT_EQ(prefixExpr->operator_, test.operator_);

    IntegerLiteral *intLit =
        dynamic_cast<IntegerLiteral *>(prefixExpr->right.get());
    EXPECT_NE(intLit, nullptr);
    EXPECT_EQ(intLit->value, test.value);
    EXPECT_EQ(intLit->TokenLiteral(), std::to_string(test.value));
  }
}

struct InfixTests {
  std::string input;
  int leftValue;
  std::string operator_;
  int rightValue;
};
TEST(Parser, TestInfixExpressionsInt) {
  std::vector<InfixTests> tests = {
      {"5 + 5;", 5, "+", 5},   {"5 - 5;", 5, "-", 5},   {"5 * 5;", 5, "*", 5},
      {"5 / 5;", 5, "/", 5},   {"5 > 5;", 5, ">", 5},   {"5 < 5;", 5, "<", 5},
      {"5 == 5;", 5, "==", 5}, {"5 != 5;", 5, "!=", 5},
  };
  for (auto &&test : tests) {
    Lexer l{test.input};
    Parser p{&l};

    std::unique_ptr<Program> program = p.parseProgram();
    EXPECT_NE(program, nullptr);
    EXPECT_EQ(program->statements.size(), 1);

    ExpressionStatement *stmt =
        dynamic_cast<ExpressionStatement *>(program->statements[0].get());
    EXPECT_NE(stmt, nullptr);

    InfixExpression *infixExpr =
        dynamic_cast<InfixExpression *>(stmt->expression.get());
    EXPECT_NE(infixExpr, nullptr);
    EXPECT_EQ(infixExpr->operator_, test.operator_);

    IntegerLiteral *left =
        dynamic_cast<IntegerLiteral *>(infixExpr->left.get());
    EXPECT_NE(left, nullptr);
    EXPECT_EQ(left->value, test.leftValue);
    EXPECT_EQ(left->TokenLiteral(), std::to_string(test.leftValue));

    IntegerLiteral *right =
        dynamic_cast<IntegerLiteral *>(infixExpr->right.get());
    EXPECT_NE(right, nullptr);
    EXPECT_EQ(right->value, test.rightValue);
    EXPECT_EQ(right->TokenLiteral(), std::to_string(test.rightValue));
  }
}

TEST(Parser, TestOperatorPrecedenceParsing) {
  struct OperatorPrecedenceTests {
    std::string input;
    std::string expected;
  };
  std::vector<OperatorPrecedenceTests> tests{
      {"-a * b", "((-a) * b)"},
      {"!-a", "(!(-a))"},
      {"a + b + c", "((a + b) + c)"},
      {"a + b - c", "((a + b) - c)"},
      {"a * b * c", "((a * b) * c)"},
      {"a * b / c", "((a * b) / c)"},
      {"a + b / c", "(a + (b / c))"},
      {"a + b * c + d / e - f", "(((a + (b * c)) + (d / e)) - f)"},
      {"3 + 4; -5 * 5", "(3 + 4)((-5) * 5)"},
      {"5 > 4 == 3 < 4", "((5 > 4) == (3 < 4))"},
      {"5 < 4 != 3 > 4", "((5 < 4) != (3 > 4))"},
      {"3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"},
  };

  for (auto &&test : tests) {
    Lexer l{test.input};
    Parser p{&l};

    std::unique_ptr<Program> program = p.parseProgram();
    EXPECT_NE(program, nullptr);
    EXPECT_EQ(program->String(), test.expected);
  }
}
