#ifndef __RAW_TERM_H__
#define __RAW_TERM_H__

#include <termios.h>

class RawTerminal {
private:
  // The user's original
  // terminal attributes.
  termios orig_termios;

public:
  RawTerminal();
  virtual ~RawTerminal();

  void enableRawMode();
  void disableRawMode();

  void setFlag(int f_num);

  void readKeyPress(char *c);

  virtual void eventLoop();
};

#endif /* end of include guard: __RAW_TERM_H__ */
