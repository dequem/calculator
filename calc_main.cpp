
#include "calc.h"
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
  std::istream* istream = &std::cin;
  std::ostream* ostream = &std::cout;

  if (argc == 2) {
    istream = new std::istringstream(argv[1]);
  } else if (argc > 2) {
    std::cout << "Error: too many arguments, one is max\n";
    return 1;
  }

  try {
    calculator::Calculator(istream, ostream);
    *ostream << std::endl;
  } catch (calculator::BadToken& e) {
    std::cout << "Error: unexpected token \"" << e.what() << "\"\n";
  } catch (std::exception& e) {
    std::cout << "Error: \"" << e.what() << "\"\n";
  }

  if (istream != &std::cin) {
    delete istream;
  }
  return 0;
}
