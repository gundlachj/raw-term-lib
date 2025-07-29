#include "terminal/terminal.hpp"

void Terminal::start() {}

void Terminal::update() {
  char c;
  readKeyPress(&c);
  display(c);

  if (c == CTRL_KEY('q')) {
    display("Quitting.");
    stop();
  }
  c = 0;
}
