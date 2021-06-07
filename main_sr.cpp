#include "server.hpp"

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  try {
    Server srv;
    CommandFunc(srv);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}