#include "computer.hpp"

#include "absearch.hpp"

Computer::Computer() {}

Computer::~Computer() { Abort(); }

void Computer::Start(SearchSettings settings) {
  Abort();

  m_search.reset(new SearchContext(settings));
  m_thread.reset(
      new std::thread([this] { iterativeSearch(this->m_search.get()); }));
  m_timer.reset(new std::thread([this] {
    using clock = std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    auto point = clock::now();

    while (duration_cast<milliseconds>(clock::now() - point).count() <
               m_search.get()->settings.time &&
           !m_search.get()->stop) {
      std::this_thread::yield();
    }
    m_search.get()->stop.stop();
  }));
}

void Computer::Abort() {
  if (m_search) {
    m_search->stop.stop();
    m_thread->join();
    m_timer->join();
    m_search.reset(nullptr);
    m_thread.reset(nullptr);
    m_timer.reset(nullptr);
  }
}
