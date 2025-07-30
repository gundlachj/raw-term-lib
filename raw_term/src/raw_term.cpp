#include "raw_term/logging.hpp"
#include "raw_term/raw_term.hpp"

#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

RawTerminal::RawTerminal() {
  if (!this->rawModeEnabled) {
    enableRawMode();
  }

  if (getWindowSize() == -1) {
    panic("window size");
  }
}

RawTerminal::~RawTerminal() {
  if (this->rawModeEnabled) {
    disableRawMode();
  }
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
  struct termios raw = this->orig_termios;

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

  this->rawModeEnabled = true;
}

void RawTerminal::disableRawMode() {
  // Set the terminal's attributes
  // to the original attributes.
  // If there is an error, panic
  // and exit the program.
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &this->orig_termios) == -1) {
    panic("tcsetattr");
  }

  this->rawModeEnabled = false;
}

int RawTerminal::getWindowSize() {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
      return -1;
    }

    return getCursorPosition(&this->screen_rows, &this->screen_cols);
  }

  this->screen_rows = ws.ws_row;
  this->screen_cols = ws.ws_col;
  return 0;
}

void RawTerminal::screenAppend(const char *s, int len) {
  // Allocate new memory or extend the current memory space
  // of the screen buf to allow for an addition of characters.
  char *newBuffer = (char *) realloc(this->screen.buf, this->screen.len + len);

  // Check if the reallocate operation failed
  if (newBuffer == NULL) return;

  // Copy the new string to the end of the old buffer
  memcpy(&newBuffer[this->screen.len], s, len);

  // Set the screen to the new buffer
  // and update the length.
  this->screen.buf = newBuffer;
  this->screen.len += len;
}

void RawTerminal::screenFreeBuffer() {
  free(this->screen.buf);
}

// TODO: Implement configuration for setting flags
void RawTerminal::setFlag(int f_num) {}

void RawTerminal::readKeyPress(char *c) {
  // Read 1 byte into the variable c
  // If there is an error, panic
  // and exit the program.
  // Checking for EAGAIN allows
  // proper handling in Cygwin.
  if (read(STDIN_FILENO, c, 1) == -1 && errno != EAGAIN) {
    panic("read");
  }
}

int RawTerminal::getCursorPosition(int *row, int *col) {
  char buf[32];
  unsigned int i = 0;

  // Queries for the cursor position and puts the
  // response in the standard input.
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
    return -1;
  }

  while (i < sizeof(buf) - 1) {
    // Read the cursor position query from stdin
    if (read(STDIN_FILENO, &buf[i], 1) != 1)
      break;
    // Breaks if the last character is found
    if (buf[i] == 'R')
      break;
    i++;
  }
  buf[i] = '\0';

  // Checks to see if the response
  // is a proper escape sequence.
  if (buf[0] != '\x1b' || buf[1] != '[') {
    return -1;
  }

  // Splits the response by the semi-colon
  // and puts the numbers in row and col.
  if (sscanf(&buf[2], "%d;%d", row, col) != 2) {
    return -1;
  }

  return 0;
}

void RawTerminal::refreshScreen() {
  this->screen = SCREEN_BUF_INIT;

  // Hide the cursor
  screenAppend("\x1b[?25l", 6);

  // Clear the screen
  screenAppend("\x1b[2J", 4);
  // Move the cursor to the top
  screenAppend("\x1b[H", 3);

  // Show the cursor
  screenAppend("\x1b[?25h", 6);

  writeScreen();
  screenFreeBuffer();
}

void RawTerminal::writeScreen() {
  write(STDOUT_FILENO, this->screen.buf, this->screen.len);
}

// TODO: Implement an actual display
void RawTerminal::display(const char screen) {
  // iscntrl checks if the char
  // is a nonprintable control
  // ASCII character (0-31, 127).
  if (iscntrl(screen)) {
    // Prints the char in its
    // decimal representation.
    //
    // Need to add carriage returns
    // as OPOST is disabled.
    printf("%d\r\n", screen);
  } else {
    // Prints the char in both
    // decimal and character
    // representation.
    //
    // Need to add carriage returns
    // as OPOST is disabled.
    printf("%d ('%c')\r\n", screen, screen);
  }
}

void RawTerminal::display(const char *screen) { printf("%s\r\n", screen); }

void RawTerminal::run() {
  start();
  this->running = true;
  while (this->running) {
    // TODO: Implement time frames system
    update();
    writeScreen();
  }
}

void RawTerminal::stop() {
  this->running = false;
  if (this->rawModeEnabled) {
    disableRawMode();
  }
}

void RawTerminal::start() {}

void RawTerminal::update() {}
