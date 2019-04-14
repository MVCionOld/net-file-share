//
// Created by Dmytro Rodionov on 2019-04-14.
//

#ifndef SRC_PROGRESSBAR_HPP
#define SRC_PROGRESSBAR_HPP


class ProgressBar {
public:

  explicit ProgressBar(size_t total);

  void publish_progress(int current);

private:
  size_t total_;
  std::chrono::time_point<std::chrono::system_clock> start_time_;
};


#endif //SRC_PROGRESSBAR_H
