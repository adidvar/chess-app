#include "pvsbenchmark.hpp"
#include "qsearchbench.hpp"
#include "searchbench.hpp"
#include "staticbenchmark.hpp"

int main() {
  QSearchBenchmark();
  PVSSearchBenchmark();
  // SearchBenchmark();
  StaticBenchmark();
  return 0;
}
