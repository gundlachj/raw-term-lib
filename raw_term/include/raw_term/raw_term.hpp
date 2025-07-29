#ifndef __RAW_TERM_H__
#define __RAW_TERM_H__

#include <termios.h>
#include <string>

#ifndef CTRL_KEY
#define CTRL_KEY(k) ((k) & 0x1f)
#endif

class RawTerminal {
private:
  // The user's original
  // terminal attributes.
  termios orig_termios;

  bool running = false;

  bool rawModeEnabled = false;

  void enableRawMode();
  void disableRawMode();

  virtual void start();

  virtual void update();

public:
  RawTerminal();
  virtual ~RawTerminal();

  void setFlag(int f_num);

  void display(const char screen);
  void display(const std::string screen);

  void readKeyPress(char *c);

  void run();

  void stop();
};

#endif /* end of include guard: __RAW_TERM_H__ */
