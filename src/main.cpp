#include "application/CApplication.hpp"


int main (int argc, char *argv[]) {

#if defined(_WIN32) || defined(_WIN64)
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2,2), &wsa_data) != 0) {
    std::cout << "Cannot link "
    << "\"ws2_32.lib\", "
    << "\"mswsock.lib\", or "
    << "\"advapi32.lib\".\n"
    << "Try to link them and restart.\n"
    << "Terminating...";
    return 0;
  }
#endif

  CApplication app;
  try {
    app(argc, argv);
  }
  catch (std::exception &exception) {
    std::cout << "Error.\n" << exception.what();
  }

#if defined(_WIN32) || defined(_WIN64)
  WSACleanup();
#endif

  return 0;
}