#include "pvsbenchmark.hpp"
#include "qsearchbench.hpp"
#include "searchbench.hpp"

int main() {
  QSearchBenchmark();
  PVSSearchBenchmark();
  SearchBenchmark();
  return 0;
}
