#include <stdexcept>

#include "argparse/argparse.hpp"
#include "receiver/CReceiver.hpp"
#include "sender/CSender.hpp"


int main (int argc, const char **argv) {
  ArgumentParser parser;
  parser.addArgument("-r", "--receive");
  parser.addArgument("-s", "--send");
  parser.addArgument("-i", "--ip");
  parser.addArgument("-f", "--file");
  parser.addArgument("-t", "--threads");
  parser.parse(static_cast<size_t>(argc), argv);
  if (parser.exists("--receive")) {
    for (const auto &param: {"-s", "-i", "-f", "-t"}) {
      if (parser.exists(param)) {
        throw std::invalid_argument("Too much arguments.");
      }
    }
    CReceiver receiver;
    receiver.Receive();
  } else if (parser.exists("--send")) {
    const bool correct_arg = parser.exists("--ip")
                             && parser.exists("--file");
    if (!correct_arg) {
      throw std::invalid_argument("No enough arguments for \"send\".");
    }
    const std::string ip = parser.retrieve<std::string>("ip");
    const std::string file_path = parser.retrieve<std::string>("file");
    const size_t threads_amt = !parser.exists("threads") ? 1
                                                         : parser.retrieve<size_t>("threads");
    CSender sender(ip);
    sender.Send(file_path, threads_amt);
  } else {
    throw std::invalid_argument("Invalid arguments.");
  }
  return 0;
}