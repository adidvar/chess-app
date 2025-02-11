#include "uci.hpp"

int main() {
  try {
    UCI uci;
    uci.loop();
  } catch (std::exception &e) {
    sendCritical(e.what());
  }
  return 0;
}
