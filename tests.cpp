
#include <iomanip>
#include <iostream>

#include "calc.h"

using namespace calculator;

// the deeper the function in call stack, the greater its output indent
int cout_indent_level_for_tests = 0;

#define TEST_INIT                                  \
  bool result = true;                              \
  cout_indent_level_for_tests += 2;                \
  std::ios::fmtflags old_flags(std::cout.flags()); \
  std::cout << std::boolalpha;

#define TEST_FUNCTION(x)                                                     \
  {                                                                          \
    bool local_result = x();                                                 \
    std::cout << std::setfill(' ') << std::setw(cout_indent_level_for_tests) \
              << ' ' << #x ": " << (local_result ? "Ok" : "FAILED")          \
              << std::endl;                                                  \
    result &= local_result;                                                  \
  }

#define TEST_RETURN                 \
  std::cout.flags(old_flags);       \
  cout_indent_level_for_tests -= 2; \
  return result;

bool TestCustomFractionToFloatZeroFraction() {
  return CustomFraction{1, 0}.ToFloat() == 1.f;
}

bool TestCustomFractionToFloatZeroInteger() {
  return CustomFraction{0, 1}.ToFloat() == .1f;
}

bool TestCustomFractionToFloatNonZero() {
  return CustomFraction{1, 1}.ToFloat() == 1.1f;
}

bool TestCustomFractionToFloatBigNumber() {
  return CustomFraction{12345, 12345}.ToFloat() == 12345.12345f;
}

bool TestCustomFractionToFloatNegativeNumber() {
  return CustomFraction{-1, 1}.ToFloat() == -1.1f;
}

bool TestCustomFractionInputOperatorDot() {
  std::istringstream stream("12.34");
  CustomFraction f{0, 0};
  stream >> f;
  return (f.integer == 12) && (f.fraction == 34);
}

bool TestCustomFractionInputOperatorComma() {
  std::istringstream stream("12,34");
  CustomFraction f{0, 0};
  stream >> f;
  return (f.integer == 12) && (f.fraction == 34);
}

bool TestCustomFractionInputOperatorSpaces() {
  std::istringstream stream("  12 ,  34  ");
  CustomFraction f{0, 0};
  stream >> f;
  return (f.integer == 12) && (f.fraction == 34);
}

bool CalculatorResultIsAsExpected(const std::string& input_string,
                                  const std::string& wanted_result) {
  std::istream* istream = new std::istringstream(input_string);
  std::ostream* ostream = new std::ostringstream(std::string());
  calculator::Calculator(istream, ostream);
  bool is_equal = dynamic_cast<std::ostringstream*>(ostream)->str() == wanted_result;
  delete istream;
  delete ostream;
  return is_equal;
}


bool TestCalculatorExample1() {
  return CalculatorResultIsAsExpected("-1 + 5 - 3", "1");
}

bool TestCalculatorExample2() {
  return CalculatorResultIsAsExpected("-10 + (8 * 2.5) - (3 / 1,5)", "8");
}

bool TestCalculatorExample3() {
  return CalculatorResultIsAsExpected("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)", "11");
}

bool TestCalculatorExample4() {
  std::istream* istream = new std::istringstream("1.1 + 2.1 + abc");
  std::ostream* ostream = new std::ostringstream(std::string());
  bool is_ok;
  try {
    calculator::Calculator(istream, ostream);
  } catch (calculator::BadToken& e) {
    is_ok = std::string(e.what()) == "abc";
  }
  delete istream;
  delete ostream;
  return is_ok;
}

bool TestRoundLonger() {
  return CalculatorResultIsAsExpected("123.123456", "123.12");
}

bool TestRoundLongerAboveMiddle() {
  return CalculatorResultIsAsExpected("123.1250001", "123.13");
}

bool TestRoundShorter() {
  return CalculatorResultIsAsExpected("123.1", "123.1");
}

bool TestFail() {
  return !CalculatorResultIsAsExpected("123.123456", "123456");
}

bool TestCustomFractionToFloat() {
  TEST_INIT;
  TEST_FUNCTION(TestCustomFractionToFloatZeroFraction);
  TEST_FUNCTION(TestCustomFractionToFloatZeroInteger);
  TEST_FUNCTION(TestCustomFractionToFloatNonZero);
  TEST_FUNCTION(TestCustomFractionToFloatNegativeNumber);
  TEST_FUNCTION(TestCustomFractionToFloatBigNumber);
  TEST_RETURN;
}

bool TestCustomFractionInputOperator() {
  TEST_INIT;
  TEST_FUNCTION(TestCustomFractionInputOperatorDot);
  TEST_FUNCTION(TestCustomFractionInputOperatorComma);
  TEST_FUNCTION(TestCustomFractionInputOperatorSpaces);
  TEST_RETURN;
}

bool TestCalculator() {
  TEST_INIT;
  TEST_FUNCTION(TestCalculatorExample1);
  TEST_FUNCTION(TestCalculatorExample2);
  TEST_FUNCTION(TestCalculatorExample3);
  TEST_FUNCTION(TestCalculatorExample4);
  TEST_RETURN;
}

bool TestRound() {
  TEST_INIT;
  TEST_FUNCTION(TestRoundLonger);
  TEST_FUNCTION(TestRoundLongerAboveMiddle);
  TEST_FUNCTION(TestRoundShorter);
  TEST_RETURN;
}

bool TestAll() {
  TEST_INIT;
  std::cout << "Testing started:" << std::endl;
  TEST_FUNCTION(TestCustomFractionToFloat);
  TEST_FUNCTION(TestCustomFractionInputOperator);
  TEST_FUNCTION(TestCalculator);
  TEST_FUNCTION(TestRound);
  TEST_FUNCTION(TestFail);
  std::cout << "All tests: " << (result ? "Ok" : "FAILED") << std::endl;
  TEST_RETURN;
}

int main() { return TestAll() ? 0 : -1; }
