#ifndef __RAW_TERM_H__
#define __RAW_TERM_H__

#include <termios.h>

class RawTerminal {
private:
  // The user's original
  // terminal attributes.
  termios orig_termios;

  bool running;

  void enableRawMode();
  void disableRawMode();

  virtual void start();

  virtual void update();

public:
  RawTerminal();
  virtual ~RawTerminal();

  void setFlag(int f_num);

  // TODO: Implement a screen class
  void display(char screen);

  char readKeyPress();

  void run();

  void stop();
};

#endif /* end of include guard: __RAW_TERM_H__ */
