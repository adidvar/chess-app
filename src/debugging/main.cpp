#include <alphabeta.hpp>
#include <chrono>
#include <computer.hpp>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>

int main() {
  std::string fen;
  std::getline(std::cin, fen);
  BitBoard board(fen);
  auto value = Evaluate::Value(board, Color::kWhite);
  std::cout << value.ToCentiPawns() << std::endl;
}
