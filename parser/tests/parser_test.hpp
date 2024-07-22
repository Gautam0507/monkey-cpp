#pragma once
#include "../../lexer/lexer.hpp"
#include "../parser.hpp"
#include <memory>
#include <string>
#include <vector>

bool TestIntegerLiteral(std::unique_ptr<Expression> &exp, int value);

bool testIdentifier(std::unique_ptr<Expression> &exp, std::string value);

template <typename T> bool TestLiteralExpression(Expression *exp, T type);

std::string PrintErrors(std::vector<std::string> &errors);

template <typename T1, typename T2>
bool TestInfixExpression(std::unique_ptr<Expression> &exp, std::string left,
                         std::string op, std::string right);
