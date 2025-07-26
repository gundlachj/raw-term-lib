#ifndef __TERMINAL_H
#define __TERMINAL_H

#include "raw_term/raw_term.hpp"

class Terminal : public RawTerminal {
private:
  void start();
  void update();
};

#endif /* end of include guard: __TERMINAL_H */
