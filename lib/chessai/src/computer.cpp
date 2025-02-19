#include "computer.hpp"

#include "absearch.hpp"

Computer::Computer() {}

Computer::~Computer() { Abort(); }

void Computer::Start(SearchSettings settings) {
  Abort();

  m_search.reset(new SearchContext(settings));
  m_thread.reset(
      new std::thread([this] { iterativeSearch(this->m_search.get()); }));
}

void Computer::Abort() {
  if (m_search) {
    m_search->stop.stop();
    m_thread->join();
    m_search.reset(nullptr);
    m_thread.reset(nullptr);
  }
}
