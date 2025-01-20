#include "computer.hpp"

#include "alphabeta.hpp"

Computer::Computer() {}

Computer::~Computer() { Abort(); }

void Computer::SetBoard(const BitBoard &board) { m_board = board; }

void Computer::Start() {
  AlphaBeta search(m_board, m_board.getCurrentSide());
  search.iterativeSearch();
}

void Computer::Abort() {}
