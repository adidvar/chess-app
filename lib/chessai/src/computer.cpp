#include "computer.hpp"

#include <iostream>

#include "itdeepening.hpp"

Computer::Computer(Color color) : m_color(color) {}

Computer::~Computer() { Stop(); }

void Computer::SetBoard(const BitBoard &board) { m_board = board; }

void Computer::Start() {
  if (m_thread != nullptr) return;
  m_stop_flag = false;

  m_thread = new std::thread([this]() {
    m_stat.Clear();
    ItDeepening<AlphaBeta> search(m_color);
    search.SetTTable(&m_table);
    search.SetStopFlag(&m_stop_flag);

    int max_depth = 20;

    m_value = search.GetValue(m_board, max_depth);
    search.SetStopFlag(nullptr);
    m_stat += search.GetStatistics();

    max_depth = search.GetLastDepth();

    // std::cout << m_table.Fill() << " " << m_table.Used() << std::endl;

    m_turn = search.GetTurn(m_board, max_depth);
    m_stat += search.GetStatistics();
    // m_pv = search.FindPV(m_board, max_depth);
    m_depth = max_depth;
    m_stat += search.GetStatistics();
  });
}

void Computer::Stop() {
  if (m_thread == nullptr) return;

  m_stop_flag = true;

  if (m_thread->joinable()) m_thread->join();
  delete m_thread;
  m_thread = nullptr;
}

std::vector<Turn> Computer::GetPV() const { return m_pv; }

Evaluate Computer::GetValue() const { return m_value; }

Turn Computer::GetTurn() const { return m_turn; }

int Computer::GetDepth() const { return m_depth; }

Statistics Computer::GetStatistics() { return m_stat; }
