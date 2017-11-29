
/*
    program:
  END			   // END is end-of-input
  expr_list END

    expr_list:
  expression PRINT	   // PRINT is semicolon
  expression PRINT expr_list

    expression:
  expression + term
  expression - term
  term

    term:
  term / primary
  term * primary
  primary

    primary:
  NUMBER
  NAME
  NAME = expression
  - primary
  ( expression )
*/

#include "calc.h"
#include <cctype>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace calculator;

int calculator::GetDenominatorSize(int x) {
  x = abs(x);
  return (x < 10 ? 10 :
        (x < 100 ? 100 :
        (x < 1000 ? 1000 :
        (x < 10000 ? 10000 :
        (x < 100000 ? 100000 :
        (x < 1000000 ? 1000000 :
        (x < 10000000 ? 10000000 :
        (x < 100000000 ? 100000000 :
        (x < 1000000000 ? 1000000000 :
        -1)))))))));
}

float CustomFraction::ToFloat() {
  return (float)integer +
         (-1 + 2 * (integer >= 0)) * (float)fraction /
             (float)GetDenominatorSize(fraction);
}

std::istream& calculator::operator>>(std::istream& stream,
                                     CustomFraction& fraction) {
  CustomFraction f{0, 0};
  stream >> f.integer;
  char c = 0;
  do {
    stream.get(c);
  } while (c != '\n' && isspace(c));

  if (!stream.eof()) {
    if (c == '.' || c == ',') {
      do {
        stream.get(c);
      } while (c != '\n' && isspace(c));
      if (isdigit(c)) {
        stream.putback(c);
        stream >> f.fraction;
      } else {
        stream.putback(c);
      }
    } else {
      stream.putback(c);
    }
  }
  fraction = f;
  return stream;
}

TokenValue GetToken(ParsingContext& context) {
  char c = 0;

  do {
    if (!context.istream->get(c)) {
      return context.current_token = kEndToken;
    }
  } while (c != '\n' && isspace(c));

  switch (c) {
    case ';':
    case '\n':
      return context.current_token = kPrintToken;
    case '*':
    case '/':
    case '+':
    case '-':
    case '(':
    case ')':
    case '=':
      return context.current_token = TokenValue(c);
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case ',':
      context.istream->putback(c);
      *context.istream >> context.number_value;
      return context.current_token = kNumberToken;
    default:
      if (isalpha(c)) {
        context.string_value = c;
        while (context.istream->get(c) && isalnum(c)) {
          // context.string_value += c;
          context.string_value.push_back(c);
        }
        context.istream->putback(c);
        return context.current_token = kNameToken;
      }
      throw BadToken(std::string(1, c).c_str());
      return context.current_token = kPrintToken;
  }
}

float expr(ParsingContext& context, bool get);

float prim(ParsingContext& context, bool get) {  // handle primaries
  if (get) {
    GetToken(context);
  }

  switch (context.current_token) {
    case kNumberToken:  // floating-point constant
    {
      float v = context.number_value.ToFloat();

      GetToken(context);
      return v;
    }
    case kNameToken: {
      float v = 0.;
      auto token_iterator = context.table.find(context.string_value);
      if (token_iterator != context.table.end()) {
        v = token_iterator->second;
      } else {
        throw BadToken(context.string_value.c_str());
      }
      if (GetToken(context) == kAssignToken) {
        v = expr(context, true);
      }
      return v;
    }
    case kMinusToken:  // unary minus
      return -prim(context, true);
    case kLeftParenthesisToken: {
      float e = expr(context, true);
      if (context.current_token != kRightParenthesisToken) {
        throw ParenthesisMismatch();
      }
      GetToken(context);  // eat ')'
      return e;
    }
    default:
      throw PrimaryExpected();
  }
}

float term(ParsingContext& context, bool get) {  // multiply and divide
  float left = prim(context, get);

  for (;;) {
    switch (context.current_token) {
      case kMultiplicationToken:
        left *= prim(context, true);
        break;
      case kDivisionToken:
        if (float d = prim(context, true)) {
          left /= d;
          break;
        }
        throw DivideByZero();
      default:
        return left;
    }
  }
}

float expr(ParsingContext& context, bool get) {
  float left = term(context, get);

  for (;;) switch (context.current_token) {
      case kPlusToken:
        left += term(context, true);
        break;
      case kMinusToken:
        left -= term(context, true);
        break;
      default:
        return left;
    }
}

void calculator::Calculator(std::istream* istream, std::ostream* ostream) {
  ParsingContext context;
  context.istream = istream;
  context.ostream = ostream;
  context.table["pi"] = 3.1415926535897932385;
  context.table["e"] = 2.7182818284590452354;
  while (context.istream) {
    GetToken(context);
    if (context.current_token == kEndToken) {
      break;
    }
    if (context.current_token == kPrintToken) {
      continue;
    }
    *context.ostream << expr(context, false);
  }
}
