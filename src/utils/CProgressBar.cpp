#include "CProgressBar.hpp"


CProgressBar::CProgressBar (size_t total)
    : total_(total), start_time_(std::chrono::system_clock::now()) {}

void CProgressBar::PublishProgress (size_t current) {
  char buffer[ProgressBarVal::BUFFER_SIZE];
  memset(buffer, 0, sizeof(buffer));
  memset(buffer, '#', (current * 100) / total_);
  size_t time = std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::system_clock::now() - start_time_
  ).count();
  char current_time[ProgressBarVal::BUFFER_SIZE];
  sprintf(
      current_time,
      "%02lu:%02lu:%02lu",
      time / 3600,
      (time / 60) % 60,
      time % 60
  );
#ifndef UNIX
  system("cls");
#endif
  printf(
#ifdef UNIX
      "[%-100s] (%lu%%) (%lu/%lu) (Time spent: %s)\r\n",
#else
      "[%-100s] (%lu%%) (%lu/%lu) (Time spent: %s)\n",
#endif
      buffer,
      (current * 100) / total_,
      current, total_,
      current_time
  );
  fflush(stdout);
}
