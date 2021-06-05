#include "client.hpp"

void CommandFunc(Client *client) {
  std::string command;
  char value = 0;
  std::string ip;
  uint16_t port = 0;
  std::string msg;
  bool connected = false;

  while (true) {
    std::cout << "Client> ";
    std::cin >> command;
    if (command == "help") {
      Client::HelpBox();
    } else if (command == "connect" && !connected) {
      do {
        std::cin >> value;
        ip.push_back(value);
      } while (value != ':');

      ip.pop_back();
      std::cin >> port;

      client->ConnectToServer(ip, port);
      connected = true;

    } else if (command == "send" && connected) {
      std::cin.ignore(1, ' ');
      std::getline(std::cin, msg);
      client->WriteFunc(msg);

    } else if (command == "exit") {
      return;
    } else {
      std::cerr << "If you need help enter - help" << std::endl;
      std::getline(std::cin, command);
    }
    command.erase(command.begin(), command.end());
  }
}
