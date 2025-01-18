#pragma once

#include <cstdint>
#include <vector>

class Statistics {
 public:
 private:
  std::vector<size_t> m_nodes_distribution;
  std::vector<size_t> m_pv_distribution;
  size_t m_total_nodes;
  size_t m_qsearch_notes;

  size_t m_transposition_size;
  size_t m_transposition_filled;
  size_t m_transposition_search_calls;
  size_t m_transposition_success_search_calls;
};
