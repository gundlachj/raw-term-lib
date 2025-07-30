#include "terminal/terminal.hpp"

void Terminal::start() {}

void Terminal::update() {
  refreshScreen();
  display(this->c);

  this->c = 0;
  while (this->c == 0) {
    readKeyPress(&this->c);

    if (this->c == CTRL_KEY('q')) {
      display("Quitting.");
      stop();
    }
  }

}
