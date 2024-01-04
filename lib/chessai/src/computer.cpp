#include "computer.hpp"

#include <iostream>

Computer::Computer(Color color) : m_color(color) {}

Computer::~Computer() { Wait(); }

void Computer::SetBoard(const BitBoard &board) { m_board = board; }

void Computer::Start() {
  /*
  if (m_thread) return;

  m_flag = false;
  m_thread = new std::thread(&Computer::Work, this);
*/
}

void Computer::Stop() {
  /*
  m_flag = true;
  m_thread->join();
*/
}

void Computer::Wait() {}

bool Computer::IsReady() { return false; }

Turn Computer::Get() { return {}; }
