#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <atomic>
#include <thread>

#include "itdeepening.hpp"
#include "score.hpp"
#include "statistics.hpp"
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
    m_flag = true;
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;
  }
  Turn GetTurn() { return m_turn; }

 private:
  void Work() {
    ItDeepening search(m_board, m_color);
    search.SetTTable(m_table);
    search.SetStopFlag(nullptr);
    for (int i = 1; i < 8; i++) {
      auto value = search.GetValue(i);
      m_turn = search.GetTurn();
      std::cout << value.ToCentiPawns() << " " << m_turn.ToChessFormat()
                << std::endl;
    }
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
  Computer(Color color);
  ~Computer();

  void SetBoard(const BitBoard &board);

  void Start();
  void Stop();

  void Wait();
  bool IsReady();

  Turn Get();

 private:
  std::thread *m_thread;
  std::atomic_bool m_flag;
  Turn m_turn;

  TTable m_table;
  BitBoard m_board;
  Color m_color;
};

#endif
