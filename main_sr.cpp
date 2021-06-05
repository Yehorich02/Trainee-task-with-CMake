#include "server.hpp"

int main(int, char *[]) {
  try {
    Server srv;
    CommandFunc(srv);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}