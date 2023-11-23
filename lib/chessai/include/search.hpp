#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <atomic>
#include <bftable.hpp>
#include <figures.hpp>
#include <statistics.hpp>

struct SearchExitException {};

class Search {
 public:
  Search(Color color) : m_color(color) {}

  [[nodiscard]] std::atomic_bool *GetStopFlag() const { return m_stop_flag; };
  void SetStopFlag(std::atomic_bool *stop_flag) { m_stop_flag = stop_flag; };

  void CheckStopFlag() {
    if ((m_stop_flag != nullptr) && (m_stop_flag->load()))
      throw SearchExitException{};
  };

  [[nodiscard]] Statistics GetStatistics() const { return m_stat; };

 private:
  std::atomic_bool *m_stop_flag = nullptr;

 protected:
  Statistics m_stat;
  Color m_color;
};

#endif
