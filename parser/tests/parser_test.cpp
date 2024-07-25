#include "../../lexer/lexer.hpp"
#include "../parser.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct testIdentifierstruct {
  std::string ExpectedIdentifier;

  testIdentifierstruct(std::string expectedIdentifier)
      : ExpectedIdentifier(expectedIdentifier) {}
};

struct PrefixTests {
  std::string input;
  std::string operator_;
  int value;
};

bool TestIntegerLiteral(Expression *exp, int value) {
  IntegerLiteral *intLit = dynamic_cast<IntegerLiteral *>(exp);
  if (intLit == nullptr) {
    return false;
  }
  if (intLit->value != value) {
    return false;
  }
  if (intLit->TokenLiteral() != std::to_string(value)) {
    return false;
  }
  return true;
}

bool testIdentifier(Expression *exp, std::string value) {
  Identifier *ident = dynamic_cast<Identifier *>(exp);
  if (ident == nullptr) {
    return false;
  }
  if (ident->value != value) {
    return false;
  }
  if (ident->TokenLiteral() != value) {
    return false;
  }
  return true;
}
bool TestBooleanLiteral(Expression *exp, bool value) {
  Boolean *boolean = dynamic_cast<Boolean *>(exp);
  if (boolean == nullptr) {
    return false;
  }
  if (boolean->value != value) {
    return false;
  }
  if (boolean->TokenLiteral() != (value ? "true" : "false")) {
    return false;
  }
  return true;
}

template <typename T> bool TestLiteralExpression(Expression *exp, T type) {
  if constexpr (std::is_same_v<T, int>) {
    return TestIntegerLiteral(exp, type);
  } else if constexpr (std::is_same_v<T, std::string>) {
    return testIdentifier(exp, type);
  } else if (std::is_same_v<T, bool>) {
    return TestBooleanLiteral(exp, type);
  }
  std::cerr << "Type of exp not handled " << std::endl;
  return false;
}

std::string PrintErrors(std::vector<std::string> &errors) {
  std::string errorString;
  for (auto &&error : errors) {
    errorString += error + "\n";
  }
  return errorString;
}

template <typename T1, typename T2>
bool testInfixExpression(Expression *expression, T1 left, const std::string &op,
                         T2 right) {
  InfixExpression *infixExpression =
      dynamic_cast<InfixExpression *>(expression);
  if (infixExpression == nullptr) {
    return false;
  }
  if (!testLiteralExpression(infixExpression->left.get(), left)) {
    return false;
  }
  if (infixExpression->operator_ != op) {
    return false;
  }
  if (!testLiteralExpression(infixExpression->right.get(), right)) {
    return false;
  }
  return true;
}

TEST(Parser, LetStatementTest) {
  std::string input{"let x = 5;"
                    "let y = 10;"
                    "let foobar = 838383;"};
  Lexer l{input};
  Parser p{&l};

  std::vector<testIdentifierstruct> tests = {
      {"x"},
      {"y"},
      {"foobar"},
  };

  std::unique_ptr<Program> program = p.parseProgram();
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

  std::vector<testIdentifierstruct> tests = {
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

  EXPECT_TRUE(TestIntegerLiteral(stmt->expression.get(), 5));
}

TEST(Parser, TestParsingPrefixExpressionsint) {
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

    EXPECT_TRUE(TestLiteralExpression(prefixExpr->right.get(), test.value));
  }
}

TEST(Parser, TestParsingPrefixExpresssionsBool) {
  struct PrefixBoolTests {
    std::string input;
    std::string operator_;
    bool value;
  };

  std::vector<PrefixBoolTests> tests = {
      {"!true;", "!", true},
      {"!false;", "!", false},
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

    Boolean *boolean = dynamic_cast<Boolean *>(prefixExpr->right.get());
    EXPECT_NE(boolean, nullptr);
    EXPECT_EQ(boolean->value, test.value);
    EXPECT_EQ(boolean->TokenLiteral(), test.value ? "true" : "false");
  }
}

struct InfixTests {
  std::string input;
  int leftValue;
  std::string operator_;
  int rightValue;
};
TEST(Parser, TestInfixExpressions) {
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

    if (!TestLiteralExpression(infixExpr->left.get(), test.leftValue)) {
      FAIL();
    }
    if (!TestLiteralExpression(infixExpr->right.get(), test.rightValue)) {
      FAIL();
    }
  }
}

TEST(Parser, TestInfixExpressionsBool) {
  struct InfixBoolTests {
    std::string input;
    bool leftValue;
    std::string operator_;
    bool rightValue;
  };

  std::vector<InfixBoolTests> tests = {
      {"true == true", true, "==", true},
      {"true != false", true, "!=", false},
      {"false == false", false, "==", false},
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

    Boolean *left = dynamic_cast<Boolean *>(infixExpr->left.get());
    EXPECT_NE(left, nullptr);
    EXPECT_EQ(left->value, test.leftValue);
    EXPECT_EQ(left->TokenLiteral(), test.leftValue ? "true" : "false");

    Boolean *right = dynamic_cast<Boolean *>(infixExpr->right.get());
    EXPECT_NE(right, nullptr);
    EXPECT_EQ(right->value, test.rightValue);
    EXPECT_EQ(right->TokenLiteral(), test.rightValue ? "true" : "false");

    if (!TestLiteralExpression(infixExpr->left.get(), test.leftValue)) {
      FAIL();
    }
    if (!TestLiteralExpression(infixExpr->right.get(), test.rightValue)) {
      FAIL();
    }
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
      {"true", "true"},
      {"false", "false"},
      {"3 > 5 == false", "((3 > 5) == false)"},
      {"3 < 5 == true", "((3 < 5) == true)"},
      {"1 + (2 + 3) + 4", "((1 + (2 + 3)) + 4)"},
      {"(5 + 5) * 2", "((5 + 5) * 2)"},
      {"2 / (5 + 5)", "(2 / (5 + 5))"},
      {"-(5 + 5)", "(-(5 + 5))"},
      {"!(true == true)", "(!(true == true))"},
      {"a + add(b * c) + d", "((a + add((b * c))) + d)"},
      {"add(a, b, 1, 2 * 3, 4 + 5, add(6, 7 * 8))",
       "add(a, b, 1, (2 * 3), (4 + 5), add(6, (7 * 8)))"}};

  for (auto &&test : tests) {
    Lexer l{test.input};
    Parser p{&l};

    std::unique_ptr<Program> program = p.parseProgram();
    EXPECT_NE(program, nullptr);
    EXPECT_EQ(program->String(), test.expected);
  }
}

TEST(Parser, TestBooleanParseing) {
  std::string input{"true;"};
  Lexer l{input};
  Parser p{&l};

  std::unique_ptr<Program> program = p.parseProgram();
  EXPECT_NE(program, nullptr);
  EXPECT_EQ(program->statements.size(), 1);

  ExpressionStatement *stmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  EXPECT_NE(stmt, nullptr);

  Boolean *boolean = dynamic_cast<Boolean *>(stmt->expression.get());
  EXPECT_NE(boolean, nullptr);
  EXPECT_EQ(boolean->value, true);
  EXPECT_EQ(boolean->TokenLiteral(), "true");
}

TEST(Parser, TestIfExpression) {
  std::string input{"if (x < y) { x }"};
  Lexer l{input};
  Parser p{&l};

  std::unique_ptr<Program> program = p.parseProgram();
  EXPECT_NE(program, nullptr);
  EXPECT_EQ(program->statements.size(), 1);

  ExpressionStatement *stmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  EXPECT_NE(stmt, nullptr);

  IfExpression *ifExpr = dynamic_cast<IfExpression *>(stmt->expression.get());
  EXPECT_NE(ifExpr, nullptr);

  InfixExpression *infixExpr =
      dynamic_cast<InfixExpression *>(ifExpr->condition.get());
  EXPECT_NE(infixExpr, nullptr);
  EXPECT_EQ(infixExpr->operator_, "<");

  Identifier *ident = dynamic_cast<Identifier *>(infixExpr->left.get());
  EXPECT_NE(ident, nullptr);
  EXPECT_EQ(ident->value, "x");

  ident = dynamic_cast<Identifier *>(infixExpr->right.get());
  EXPECT_NE(ident, nullptr);
  EXPECT_EQ(ident->value, "y");

  BlockStatement *blockStmt =
      dynamic_cast<BlockStatement *>(ifExpr->consequence.get());
  EXPECT_NE(blockStmt, nullptr);
  EXPECT_EQ(blockStmt->statements.size(), 1);

  ExpressionStatement *blockStmtExpr =
      dynamic_cast<ExpressionStatement *>(blockStmt->statements[0].get());
  EXPECT_NE(blockStmtExpr, nullptr);

  ident = dynamic_cast<Identifier *>(blockStmtExpr->expression.get());
  EXPECT_NE(ident, nullptr);
  EXPECT_EQ(ident->value, "x");
}

TEST(Parser, TestFunctionLiteralParsing) {
  std::string input{"fn(x, y) { x + y; }"};
  Lexer l{input};
  Parser p{&l};

  std::unique_ptr<Program> program = p.parseProgram();
  EXPECT_NE(program, nullptr);
  EXPECT_EQ(program->statements.size(), 1);

  ExpressionStatement *stmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  EXPECT_NE(stmt, nullptr);

  FunctionLiteral *fn = dynamic_cast<FunctionLiteral *>(stmt->expression.get());
  EXPECT_NE(fn, nullptr);

  EXPECT_EQ(fn->parameters.size(), 2);
  EXPECT_EQ(fn->parameters[0]->value, "x");
  EXPECT_EQ(fn->parameters[1]->value, "y");

  BlockStatement *blockStmt = dynamic_cast<BlockStatement *>(fn->body.get());
  EXPECT_NE(blockStmt, nullptr);
  EXPECT_EQ(blockStmt->statements.size(), 1);

  ExpressionStatement *blockStmtExpr =
      dynamic_cast<ExpressionStatement *>(blockStmt->statements[0].get());
  EXPECT_NE(blockStmtExpr, nullptr);

  InfixExpression *infixExpr =
      dynamic_cast<InfixExpression *>(blockStmtExpr->expression.get());
  EXPECT_NE(infixExpr, nullptr);

  Identifier *identLeft = dynamic_cast<Identifier *>(infixExpr->left.get());
  EXPECT_NE(identLeft, nullptr);
  EXPECT_EQ(identLeft->value, "x");

  Identifier *identRight = dynamic_cast<Identifier *>(infixExpr->right.get());
  EXPECT_NE(identRight, nullptr);
  EXPECT_EQ(identRight->value, "y");
}

TEST(Parser, TestFunctionParameterParsing) {
  struct FunctionParameterTests {
    std::string input;
    std::vector<std::string> expectedParams;
  };

  std::vector<FunctionParameterTests> tests = {
      {"fn() {};", {}},
      {"fn(x) {};", {"x"}},
      {"fn(x, y, z) {};", {"x", "y", "z"}},
  };

  for (auto &&test : tests) {
    Lexer l{test.input};
    Parser p{&l};

    std::unique_ptr<Program> program = p.parseProgram();
    EXPECT_NE(program, nullptr);

    ExpressionStatement *stmt =
        dynamic_cast<ExpressionStatement *>(program->statements[0].get());
    EXPECT_NE(stmt, nullptr);

    FunctionLiteral *fn =
        dynamic_cast<FunctionLiteral *>(stmt->expression.get());
    EXPECT_NE(fn, nullptr);

    EXPECT_EQ(fn->parameters.size(), test.expectedParams.size());
    for (int i{0}; i < test.expectedParams.size(); i++) {
      EXPECT_EQ(fn->parameters[i]->value, test.expectedParams[i]);
    }
  }
}

TEST(Parser, TestCallExpressionParsing) {
  std::string input{"add(1, 2 * 3, 4 + 5);"};
  Lexer l{input};
  Parser p{&l};

  std::unique_ptr<Program> program = p.parseProgram();
  EXPECT_NE(program, nullptr);
  EXPECT_EQ(program->statements.size(), 1);

  ExpressionStatement *stmt =
      dynamic_cast<ExpressionStatement *>(program->statements[0].get());
  EXPECT_NE(stmt, nullptr);

  callExpression *callExpr =
      dynamic_cast<callExpression *>(stmt->expression.get());
  EXPECT_NE(callExpr, nullptr);

  Identifier *ident = dynamic_cast<Identifier *>(callExpr->function.get());
  EXPECT_NE(ident, nullptr);
  EXPECT_EQ(ident->value, "add");

  EXPECT_EQ(callExpr->arguments.size(), 3);

  if (!TestLiteralExpression(callExpr->arguments[0].get(), 1)) {
    FAIL();
  }

  InfixExpression *infixExpr =
      dynamic_cast<InfixExpression *>(callExpr->arguments[1].get());
  EXPECT_NE(infixExpr, nullptr);
  if (!TestLiteralExpression(infixExpr->left.get(), 2)) {
    FAIL();
  }
  if (!TestLiteralExpression(infixExpr->right.get(), 3)) {
    FAIL();
  }

  infixExpr = dynamic_cast<InfixExpression *>(callExpr->arguments[2].get());
  EXPECT_NE(infixExpr, nullptr);
  if (!TestLiteralExpression(infixExpr->left.get(), 4)) {
    FAIL();
  }
  if (!TestLiteralExpression(infixExpr->right.get(), 5)) {
    FAIL();
  }
}

TEST(Parser, TestLetStatements) {
  struct LetStatementTestsint {
    std::string input;
    std::string expectedIdentifier;
    int expectedValue;
  };
  struct LetStatementTestsString {
    std::string input;
    std::string expectedIdentifier;
    std::string expectedValue;
  };
  struct LetStatementTestsBool {
    std::string input;
    std::string expectedIdentifier;
    bool expectedValue;
  };

  std::vector<LetStatementTestsint> testsInt = {
      {"let x = 5;", "x", 5},
      {"let y = 10;", "y", 10},
      {"let foobar = 838383;", "foobar", 838383},
  };

  std::vector<LetStatementTestsString> testsString = {
      {"let x = hello;", "x", "hello"},
      {"let y = world;", "y", "world"},
  };

  std::vector<LetStatementTestsBool> testsBool = {
      {"let x = true;", "x", true},
      {"let y = false;", "y", false},
  };

  for (auto &&test : testsInt) {
    Lexer l{test.input};
    Parser p{&l};

    std::unique_ptr<Program> program = p.parseProgram();
    EXPECT_NE(program, nullptr);
    EXPECT_EQ(program->statements.size(), 1);

    ExpressionStatement *stmt =
        dynamic_cast<ExpressionStatement *>(program->statements[0].get());
    EXPECT_NE(stmt, nullptr) << program->statements[0]->TokenLiteral();

    LetStatement *letStmt =
        dynamic_cast<LetStatement *>(stmt->expression.get());
    EXPECT_NE(letStmt, nullptr);
    EXPECT_EQ(letStmt->TokenLiteral(), "let");
    EXPECT_EQ(letStmt->name->value, test.expectedIdentifier);
    EXPECT_EQ(letStmt->name->TokenLiteral(), test.expectedIdentifier);
    if (!TestLiteralExpression(letStmt->value.get(), test.expectedValue)) {
      FAIL();
    }
  }

  for (auto &&test : testsString) {
    Lexer l{test.input};
    Parser p{&l};

    std::unique_ptr<Program> program = p.parseProgram();
    EXPECT_NE(program, nullptr);
    EXPECT_EQ(program->statements.size(), 1);

    ExpressionStatement *stmt =
        dynamic_cast<ExpressionStatement *>(program->statements[0].get());
    EXPECT_NE(stmt, nullptr);

    LetStatement *letStmt =
        dynamic_cast<LetStatement *>(stmt->expression.get());
    EXPECT_NE(letStmt, nullptr);
    EXPECT_EQ(letStmt->TokenLiteral(), "let");
    EXPECT_EQ(letStmt->name->value, test.expectedIdentifier);
    EXPECT_EQ(letStmt->name->TokenLiteral(), test.expectedIdentifier);
    if (!TestLiteralExpression(letStmt->value.get(), test.expectedValue)) {
      FAIL();
    }
  }

  for (auto &&test : testsBool) {
    Lexer l{test.input};
    Parser p{&l};

    std::unique_ptr<Program> program = p.parseProgram();
    EXPECT_NE(program, nullptr);
    EXPECT_EQ(program->statements.size(), 1);

    ExpressionStatement *stmt =
        dynamic_cast<ExpressionStatement *>(program->statements[0].get());
    EXPECT_NE(stmt, nullptr);

    LetStatement *letStmt =
        dynamic_cast<LetStatement *>(stmt->expression.get());
    EXPECT_NE(letStmt, nullptr);
    EXPECT_EQ(letStmt->TokenLiteral(), "let");
    EXPECT_EQ(letStmt->name->value, test.expectedIdentifier);
    EXPECT_EQ(letStmt->name->TokenLiteral(), test.expectedIdentifier);
    if (!TestLiteralExpression(letStmt->value.get(), test.expectedValue)) {
      FAIL();
    }
  }
}
