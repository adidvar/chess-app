#include "computer.hpp"

#include <iostream>

#include "itdeepening.hpp"

Computer::Computer(Color color) : m_color(color) {}

Computer::~Computer() { Wait(); }

void Computer::SetBoard(const BitBoard &board) { m_board = board; }

void Computer::Start() {}

void Computer::Wait() {}

bool Computer::IsReady() { return false; }

Turn Computer::Get() { return {}; }

Turn Computer::Work() {
  ItDeepening search(m_board, m_color);
  search.SetStopFlag(nullptr);
  search.SetTTable(&m_table);

  search.GetValue(9);
  return search.GetTurn();
}
