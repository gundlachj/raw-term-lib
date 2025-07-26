#include "raw_term/logging.hpp"
#include "raw_term/raw_term.hpp"

#include <cctype>
#include <cerrno>
#include <cstdlib>

#include <termios.h>
#include <unistd.h>

RawTerminal::RawTerminal() {
  enableRawMode();
}

RawTerminal::~RawTerminal() {
  disableRawMode();
}

void RawTerminal::enableRawMode() {
  // Get the user's original
  // terminal attributes.
  // If there is an error, panic
  // and exit the program.
  if (tcgetattr(STDIN_FILENO, &this->orig_termios) == -1) {
    panic("tcgetattr");
  }
}

void RawTerminal::disableRawMode() {
  // Set the terminal's attributes
  // to the original attributes.
  // If there is an error, panic
  // and exit the program.
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &this->orig_termios) == -1) {
    panic("tcsetattr");
  }
}

void RawTerminal::setFlag(int f_num) {}
