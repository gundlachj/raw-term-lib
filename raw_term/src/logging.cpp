#include "raw_term/logging.hpp"

#include <cerrno>

// TODO: Implement proper exception handling
void panic(const char *message) {
  perror(message);
  exit(1);
}
