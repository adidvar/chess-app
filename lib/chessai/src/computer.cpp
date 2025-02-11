#include "computer.hpp"

#include "alphabeta.hpp"

Computer::Computer() {}

Computer::~Computer() { Abort(); }

void Computer::Start(SearchSettings settings) {
  Abort();

  m_search.reset(new Search(settings));
  m_thread.reset(new std::thread([this] { m_search->iterativeSearch(); }));
}

void Computer::Abort() {
  if (m_search) {
    m_search->m_stop_flag.stop();
    m_thread->join();
    m_search.reset(nullptr);
    m_thread.reset(nullptr);
  }
}
