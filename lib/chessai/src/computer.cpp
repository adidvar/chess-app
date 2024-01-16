#include "computer.hpp"

#include <iostream>

#include "searchmanager.hpp"

Computer::Computer() { m_thread = nullptr; }

Computer::~Computer() { Abort(); }

void Computer::SetBoard(const BitBoard &board) { m_board = board; }

void Computer::Start() {
  if (m_thread) return;

  m_abort_flag = false;
  m_turn = {};
  m_thread = new std::thread(&Computer::Work, this);
}

void Computer::Abort() {
  if (m_thread == nullptr) return;

  m_abort_flag = true;
  if (m_thread->joinable()) m_thread->join();
  delete m_thread;
  m_thread = nullptr;
}

bool Computer::IsReady() { return m_turn != Turn{}; }

Turn Computer::Get() { return m_turn; }

void Computer::Work() {
  TTable table;
  ThreadController controller(m_board.CurrentColor(), m_board, &table);

  using timer = std::chrono::high_resolution_clock;
  auto begin = timer::now();
  auto time = std::chrono::duration<float, std::ratio<1, 1>>{6};
  controller.Start();

  while (m_abort_flag == false && timer::now() < begin + time)
    std::this_thread::yield();

  controller.Stop();
  m_turn = controller.GetTurn();
}

SearchManager &Computer::GetManager() { return m_manager; }
