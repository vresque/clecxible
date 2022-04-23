#include <core.h>

int die(const char* s, ...) {
  va_list args;
  va_start(args, s);
  char buf[255]; // We do not care for memory anymore, we are about to shutdown
  vsnprintf(buf, sizeof(buf), s, args);
  perror(buf);
  va_end(args);
  exit(1);
}
