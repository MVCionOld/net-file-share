#include "application/CApplication.hpp"

int main (int argc, char *argv[]) {
  CApplication application;
  try {
    application(argc, argv);
  }
  catch (std::exception &exception) {
    std::cout << "Error.\n" << exception.what();
  }
  return 0;
}
