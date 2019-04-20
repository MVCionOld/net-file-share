#ifndef SRC_CAPPLICATION_HPP
#define SRC_CAPPLICATION_HPP

#include <iostream>
#include <stdexcept>

#include "../argparse/argparse.hpp"
#include "../receiver/CReceiver.hpp"
#include "../sender/CSender.hpp"


class CApplication {
public:
  void operator() (int argc, char *argv[]) const;
};

#endif //SRC_CAPPLICATION_HPP
