#ifndef __RAW_TERM_H__
#define __RAW_TERM_H__

class RawTerminal {
private:
  struct termios orig_termios;

public:
  RawTerminal();
  virtual ~RawTerminal();

  void enableRawMode();
  void disableRawMode();

  void setFlag(int f_num);

  virtual void eventLoop();
};

#endif /* end of include guard: __RAW_TERM_H__ */
