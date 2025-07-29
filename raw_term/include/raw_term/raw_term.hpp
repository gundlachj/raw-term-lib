#ifndef __RAW_TERM_H__
#define __RAW_TERM_H__

#include <termios.h>

#define CTRL_KEY(k) ((k) & 0x1f)

class RawTerminal {
private:
  // The user's original
  // terminal attributes.
  termios orig_termios;

  bool running = false;

  bool rawModeEnabled = false;

  int screen_rows;
  int screen_cols;

  void enableRawMode();
  void disableRawMode();

  int getWindowSize();

  virtual void start();

  virtual void update();

public:
  RawTerminal();
  virtual ~RawTerminal();

  void setFlag(int f_num);

  void refreshScreen();

  void display(const char screen);
  void display(const char *screen);

  void readKeyPress(char *c);

  int getCursorPosition(int *row, int *col);

  void run();

  void stop();
};

#endif /* end of include guard: __RAW_TERM_H__ */
