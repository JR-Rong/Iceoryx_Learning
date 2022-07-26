#include <iostream>
#include "msg_img.hpp"

int64_t GetNs() {
  timespec tv{};
  clock_gettime(CLOCK_REALTIME, &tv);
  int64_t ns = tv.tv_sec * 1000  + tv.tv_nsec / 1000000;
  
  return ns;
}

template <typename T>
void GetNs(T& data) {
  timespec tv{};
  clock_gettime(CLOCK_REALTIME, &tv);
  int64_t ns = tv.tv_sec * 1000  + tv.tv_nsec / 1000000;
  data.header.stamp.nsec = ns;
}