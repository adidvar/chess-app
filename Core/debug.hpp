#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <position.hpp>

template <class T>
void PrintBoard(const T& board) {
  const char letters[2][8] = {" pnbrqk", " PNBRQK"};

  using namespace std;
  for (size_t i = 0; i < 17; i++) {
    cout << "-";
  }
  cout << endl;
  for (size_t i = 0; i < 8; i++) {
    cout << '|';
    for (size_t j = 0; j < 8; j++) {
      Position pos(j, i);
      auto type = board.GetCell(pos);
      cout << letters[type.color][type.type] << "|";
    }
    cout << endl;
  }
  auto flags = board.RookingFlags();
  cout << (flags.white_oo ? 'K' : '-');
  cout << (flags.white_ooo ? 'Q' : '-');
  cout << (flags.black_oo ? 'k' : '-');
  cout << (flags.black_ooo ? 'q' : '-');
  for (size_t i = 0; i < 13; i++) {
    cout << "-";
  }
  cout << endl;
}

#endif
