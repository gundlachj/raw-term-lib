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

  // Get the original terminal
  // as a termios struct.
  struct termios raw = orig_termios;

  //
  // Default Termios Flags
  //
  // Disables input echoing and
  // canonical (cooked) mode.
  //
  // Bitwise-NOT operator, "~",
  // combined with the bitwise-
  // AND operator, "&=", flips
  // each 1 to become 0.
  //
  // Bitwise-OR operator, "|=",
  // is used to set bit masks,
  // which is used to set
  // certain flags.
  //

  // c_iflag is an attribute
  // which defines certain
  // input modes.
  //
  // BRKINT is the flag for
  // flushing the input and
  // output and will often
  // send a SIGINT signal.
  //
  // ICRNL is the flag for
  // translating carriage
  // returns to newline.
  //
  // INPCK is the flag for
  // input parity checking,
  // which uses an extra
  // bit to detect
  // corrupted binary
  // data. This does not
  // apply to most modern
  // terminal emulators.
  //
  // ISTRIP is the flag for
  // stripping the 8th bit,
  // which just means it
  // sets it to 0. This is
  // already turned off
  // in most modern
  // terminal emulators.
  //
  // IXON is the flag for
  // program control flow
  // signals such as XON
  // and XOFF (Ctrl-S and
  // Ctrl-Q).
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  // c_oflag is an attribute
  // which defines certain
  // output modes.
  //
  // OPOST is a flag for
  // the post-processing
  // of carriage return
  // and newline characters.
  raw.c_oflag &= ~(OPOST);

  // c_cflag is an attribute
  // which defines certain
  // configuration options.
  //
  // CS8 sets the size of
  // the character mask
  // to 8 bits per byte.
  // Most systems are
  // already set this way.
  raw.c_cflag |= (CS8);

  // c_lflag is an attribute
  // from the termios struct,
  // which defines certain
  // miscellaneous modes.
  //
  // ECHO is the flag for
  // terminal echoing.
  //
  // ICANON is the flag for
  // canonical mode, which
  // reads by line.
  //
  // IEXTEN is the flag for
  // literal character input
  // processing (Ctrl-V).
  //
  // ISIG disables the
  // SIGINT (Ctrl-C) and
  // SIGSTP (Ctrl-Z) signals.
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);


  // c_cc is an attribute
  // that sets various
  // control characters.
  // c_cc is an array of
  // bytes that control
  // various terminal
  // settings.
  //
  // VMIN sets the minimum
  // number of bytes that
  // read() needs to read
  // before returning.
  // Setting this to 0
  // results in read()
  // immediately returning
  // after every keypress.
  //
  // VTIME sets the maximum
  // amount of time to
  // wait before read()
  // returns, in tenths of
  // a second. In this case,
  // it is set to 100 ms.
  // If read() times out,
  // it will return 0.
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  // Set terminal attributes to attributes
  // set with flags above.
  // TCSAFLUSH specifies to set the attributes
  // after all pending output has been written.
  // If there is an error, panic
  // and exit the program.
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    panic("tcsetattr");
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

// TODO: Implement configuration for setting flags
void RawTerminal::setFlag(int f_num) {}
