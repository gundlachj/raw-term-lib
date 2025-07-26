#include "terminal/terminal.hpp"

void Terminal::start() {}

void Terminal::update() {
  char c = readKeyPress();
  display(c);

  if (c == 'q') {
    stop();
  }
}
