#include <iostream>

int64_t GetNs() {
  timespec tv{};
  clock_gettime(CLOCK_REALTIME, &tv);
  int64_t ns = tv.tv_sec * 1000  + tv.tv_nsec / 1000000;
  return ns;
}
