#include "uci.hpp"

int main() {
  try {
    UCI uci;
    uci.parse();
  } catch (std::exception &e) {
    Writter::critical(e.what());
  }
  return 0;
}
