//
// Created by Dmytro Rodionov on 2019-04-14.
//

#include <chrono>
#include <cstring>
#include <cstdio>
#include "ProgressBar.hpp"

ProgressBar::ProgressBar(size_t total) : total_(total) {
  start_time_ = std::chrono::system_clock::now();
}

void ProgressBar::publish_progress(int current) {
  char buffer[PATH_MAX];
  char current_time[PATH_MAX];
  memset(buffer, 0, sizeof(buffer));
  memset(buffer, '#', (current * 100) / total_);

  size_t time = std::chrono::duration_cast<std::chrono::seconds>
          (std::chrono::system_clock::now() - start_time_).count();

  sprintf(
          current_time,
          "%02d:%02d:%02d",
          time / 3600,
          (time / 60) % 60,
          time % 60
  );

  printf("[%-100s] (%d%%) (%d/%d) (Time spent: %s)\r", buffer, (current * 100) / total_, current, total_, current_time);
}
