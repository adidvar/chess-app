#include "computer.hpp"

#include "alphabeta.hpp"

Computer::Computer() {}

Computer::~Computer() { Abort(); }

void Computer::SetBoard(const BitBoard &board) { m_board = board; }

void Computer::Start() {
  m_search.reset(new Search(m_board, m_board.getCurrentSide()));
  m_thread.reset(new std::thread([this] { m_search->iterativeSearch(); }));
}

void Computer::Abort() {
  m_search->m_stop_flag.stop();
  m_thread->join();
  m_search.reset(nullptr);
  m_thread.reset(nullptr);
}
