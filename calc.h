
#ifndef CALC_H
#define CALC_H

#include <map>
#include <stdexcept>

namespace calculator {
class PrimaryExpected : public std::runtime_error {
 public:
  PrimaryExpected() : std::runtime_error("Expected primary expression") {}
};

class ParenthesisMismatch : public std::runtime_error {
 public:
  ParenthesisMismatch() : runtime_error("Parenthesis mismatch") {}
};

class DivideByZero : public std::runtime_error {
 public:
  DivideByZero() : runtime_error("Division by zero") {}
};

class BadToken : public std::runtime_error {
 public:
  BadToken(const char* token_name) : runtime_error(token_name) {}
};

class TooManyArguments : public std::runtime_error {
 public:
  TooManyArguments() : runtime_error("One argument max") {}
};

int GetDenominatorSize(int x);

struct CustomFraction {
  int integer;
  int fraction;
  float ToFloat();
};

std::istream& operator>>(std::istream& stream, CustomFraction& fraction);

enum TokenValue {
  kNameToken,
  kNumberToken,
  kEndToken,
  kPlusToken = '+',
  kMinusToken = '-',
  kMultiplicationToken = '*',
  kDivisionToken = '/',
  kPrintToken = ';',
  kAssignToken = '=',
  kLeftParenthesisToken = '(',
  kRightParenthesisToken = ')'
};

struct ParsingContext {
  TokenValue current_token;
  CustomFraction number_value;
  std::string string_value;
  std::istream* istream;
  std::ostream* ostream;
  std::map<std::string, float> table;
};

void Calculator(std::istream* istream, std::ostream* ostream);
}

#endif
