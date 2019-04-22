#include "CApplication.hpp"

void CApplication::operator() (int argc, char *argv[]) const {
  ap::parser parser(argc, argv);
  parser.add(
      "-r",
      "--receive",
      "run program in receiver mode",
      ap::mode::BOOLEAN
  );
  parser.add(
      "-s",
      "--send",
      "run program in sending mode",
      ap::mode::BOOLEAN
  );
  parser.add(
      "-i",
      "--ip",
      "ip address"
  );
  parser.add(
      "-f",
      "--file",
      "file's path"
  );
  parser.add(
      "-t",
      "--threads",
      "amount of threads"
  );
  auto args = parser.parse();
  if (!args.parsed_successfully()) {
    throw std::invalid_argument("Invalid arguments.");
  }
  if (args["-s"] == args["-r"]) {
    throw std::invalid_argument("Undefined runtime mode.");
  }
  if (std::stoi(args["-r"])) {
    for (const auto &param: {"-i", "-f", "-t"}) {
      if (!args[param].empty()) {
        throw std::invalid_argument("Too much arguments.");
      }
    }
    CReceiver receiver;
    receiver.Receive();
  } else {
    const bool mismatch_arg = args["-i"].empty() or args["-f"].empty();
    if (mismatch_arg) {
      throw std::invalid_argument("No enough arguments for \"send\".");
    }
    const size_t threads_amt = args["-t"].empty() ? 1 : std::stoi(args["-t"]);
    CSender sender(args["-i"]);
    sender.Send(args["-f"], threads_amt);
  }
}
