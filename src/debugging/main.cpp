#include <alphabeta.hpp>
#include <chrono>
#include <computer.hpp>
#include <iomanip>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>


int main() {

  Computer computer;
  computer.SetBoard({"startpos"});
  computer.Start();
  std::cout << computer.Get().ToChessFormat() << std::endl;
  std::cout << "end of code" << std::endl;
}
