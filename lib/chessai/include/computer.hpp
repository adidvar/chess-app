#pragma once

#include <atomic>
#include <thread>

#include "itdeepening.hpp"
#include "score.hpp"
#include "timemanager.hpp"
#include "ttable.hpp"

class ThreadController {
 public:
  ThreadController(Color color, BitBoard board, TTable *table)
      : m_color(color), m_board(board), m_table(table) {}
  ~ThreadController() { Stop(); }

  void Start() {
    if (m_thread) return;

    m_flag = false;
    m_thread = new std::thread(&ThreadController::Work, this);
  }
  void Stop() {
    if (m_thread == nullptr) return;

    m_flag = true;
    if (m_thread->joinable()) m_thread->join();
    delete m_thread;
    m_thread = nullptr;
  }
  Turn GetTurn() { return m_turn; }

 private:
  void Work() {
    ItDeepening search(m_board, m_color);
    search.SetTTable(m_table);
    search.SetStopFlag(&m_flag);
    auto value = search.GetValue(20);
    m_turn = search.GetTurn();
  }

 private:
  std::thread *m_thread = nullptr;
  std::atomic_bool m_flag;
  Turn m_turn{};

  TTable *m_table;
  BitBoard m_board;
  Color m_color;
};

class Computer{
 public:
  Computer();
  ~Computer();

  void SetBoard(const BitBoard &board);

  void Start();
  void Abort();

  bool IsReady();

  Turn Get();

  void Work();

  TimeManager &GetManager();

 private:
  std::thread *m_thread;
  std::atomic_bool m_abort_flag;
  Turn m_turn{};

  BitBoard m_board;
  TimeManager m_manager;
};
