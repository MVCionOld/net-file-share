#ifndef SRC_CPROGRESSBAR_HPP
#define SRC_CPROGRESSBAR_HPP

#include <chrono>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "../handshake/glob_consts.h"


class CProgressBar {
public:
  explicit CProgressBar (size_t total);

  void PublishProgress (size_t current);

private:
  size_t total_;
  std::chrono::time_point<std::chrono::system_clock> start_time_;
};


#endif //SRC_PROGRESSBAR_H
