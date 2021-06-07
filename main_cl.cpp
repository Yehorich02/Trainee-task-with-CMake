#include "client.hpp"

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  try {
    Client cl;
    CommandFunc(&cl);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}