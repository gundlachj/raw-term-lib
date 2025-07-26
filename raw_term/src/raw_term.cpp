#include "raw_term/logging.hpp"
#include "raw_term/raw_term.hpp"

#include <cctype>
#include <cerrno>
#include <cstdlib>

#include <termios.h>
#include <unistd.h>

RawTerminal::RawTerminal() {}

void RawTerminal::enableRawMode() {}

void RawTerminal::disableRawMode() {}

void RawTerminal::setFlag() {}
