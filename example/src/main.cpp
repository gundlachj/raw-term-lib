#include <iostream>

#include "terminal/terminal.hpp"

int main(int argc, char const *argv[]) {
  std::cout << "Hello World!" << '\n';

  Terminal *terminal = new Terminal();
  terminal->run();

  delete terminal;

  return 0;
}
