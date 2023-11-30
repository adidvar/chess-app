#include "staticbenchmark.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>

#include "itdeepening.hpp"
#include "pvs.hpp"
#include "ttable.hpp"

static std::vector<std::pair<const char*, float>> stockfish_result{
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 0.08f}};

void StaticBenchmark() {
  std::string name{"StaticBenchmark"};
  const int d = 16;
  const auto delim = std::string(8 * d - name.size() + 4, '-');

  float delta = 0;
  int index = 0;

  std::cout << name << delim << std::endl;

  for (auto pair : stockfish_result) {
    TTable table;
    ItDeepening<PVS> cmp(Color::kWhite);
    cmp.SetTTable(&table);

    float value = cmp.GetValue(BitBoard{pair.first}, 5).ToCentiPawns();
    float error = value - pair.second;

    std::cout << std::setw(d) << index << std::setw(d) << pair.second
              << std::setw(d) << value << std::setw(d) << abs(error)
              << std::endl;

    delta += error * error;
    index++;
  }
  std::cout << name << delim << std::endl;

  std::cout << "Delta: " << sqrt(delta) / stockfish_result.size() << std::endl;

  std::cout << name << delim << std::endl;
}
