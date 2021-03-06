#include "server.hpp"

void CommandFunc(Server &srv) {
  std::string command;
  int port = 0;
  bool port_ = false;

  while (true) {
    std::cout << "Server> ";
    std::cin >> command;
    if (command == "SetPort") {
      std::cin >> port;
      srv.SetPort(port);
      port_ = true;
    } else if (command == "start" && port_) {
      srv.start();
    } else if (command == "help") {
      srv.HelpBox();
    } else if (command == "stop") {
      srv.stop();
      std::cout << "Server stoped on port: " << port << std::endl;
    } else if (command == "exit") {
      return;
    } else {
      std::cerr << "If you need help enter - help" << std::endl;
      std::getline(std::cin, command);
    }
    command.erase(command.begin(), command.end());
  }
}
