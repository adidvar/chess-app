#include <chrono>
#include <evaluate.hpp>
#include <functional>
#include <iomanip>
#include <iostream>
#include <statistics.hpp>
#include <turn.hpp>

class BenchmarkPrint {
 public:
  using Test = std::function<void(Statistics &, Score &, Turn &, int &depth)>;

  BenchmarkPrint(std::string name) : name(name){};

  void Add(Test test, int index) {
    tests.push_back(test);
    indexes.push_back(index);
  };

  void RunAndPrint() {
    std::cout << name
              << "-------------------------------------------------------------"
              << std::endl;

    float total_time = 0;
    Statistics total_stat{};

    const int d = 10;

    int index = 0;
    for (auto &test : tests) {
      Statistics stat;
      Score score;
      Turn turn;
      int depth;

      auto t_begin = std::chrono::high_resolution_clock::now();

      test(stat, score, turn, depth);

      auto t_end = std::chrono::high_resolution_clock::now();

      auto duration = std::chrono::duration_cast<std::chrono::duration<float>>(
          t_end - t_begin);

      total_time += duration.count();
      total_stat += stat;

      std::cout << std::setw(d) << indexes[index] << ") " << std::setw(d)
                << depth << std::setw(d) << stat.GetMainNode() << std::setw(d)
                << stat.GetEndNode() << std::setw(d) << stat.GetExtraNode()
                << std::setw(d) << duration.count() << std::setw(d)
                << score.ToCentiPawns() << std::setw(d) << turn.ToChessFormat()
                << std::endl;
      ;

      if (index != indexes.size() && indexes[index + 1] != indexes[index])
        std::cout << std::endl;
      index++;
    }
    std::cout << name
              << "-------------------------------------------------------------"
              << std::endl
              << std::setw(d) << total_stat.GetMainNode() << std::setw(d)
              << total_stat.GetEndNode() << std::setw(d)
              << total_stat.GetExtraNode() << std::setw(d) << total_time
              << std::setw(d) << std::endl

              << "-------------------------------------------------------------"
              << std::endl
              << std::endl
              << std::endl;
  }

 private:
  std::vector<Test> tests;
  std::vector<int> indexes;
  std::string name;
};
