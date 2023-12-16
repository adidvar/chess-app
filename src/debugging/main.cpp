#include <alphabeta.hpp>
#include <chrono>
#include <computer.hpp>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>

#include "pvs.hpp"

int main() {
  Computer computer(Color::kWhite);
  computer.Start();
  std::this_thread::sleep_for(std::chrono::seconds{6});
  auto begin = std::chrono::high_resolution_clock::now();
  computer.Stop();
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::duration<float>>(end -
                                                                        begin)
                   .count()
            << std::endl;
  std::cout << computer.GetValue().ToCentiPawns() << std::endl;
  std::cout << computer.GetTurn().ToChessFormat() << std::endl;
  for (auto turn : computer.GetPV()) std::cout << turn.ToChessFormat() << " ";
  std::cout << std::endl << computer.GetDepth() << std::endl;
}
