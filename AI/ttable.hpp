#ifndef TRANSPOSITIONTABLE_HPP
#define TRANSPOSITIONTABLE_HPP

#include <algorithm>
#include <numeric>
#include <optional>
#include <position_rating.hpp>

#include "bitboard.hpp"

struct SearchElement {
  bitboard_hash_t hash;
  std::pair<MainAppraiser, Turn> value = {MainAppraiser::Invalid(), Turn()};
  MainAppraiser a = MainAppraiser::Invalid();
  MainAppraiser b = MainAppraiser::Invalid();
  int depth;
};

struct QSearchElement {
  bitboard_hash_t hash;
  std::pair<MainAppraiser, Turn> value = {MainAppraiser::Invalid(), Turn()};
};

template <class T>
class BitBoardMap {
 public:
  BitBoardMap() {
    size_t mb = 64;
    size_t bytes = mb * 1024 * 1024;
    size_t size = bytes / sizeof(T);
    table_.resize(size);
    used_.resize(size);
  }

  std::optional<T> Find(bitboard_hash_t hash) {
    if (used_[hash % used_.size()] == false)
      return {};
    else
      return table_[hash % used_.size()];
  }
  void Write(bitboard_hash_t hash, const T &element) {
    table_[hash % used_.size()] = element;
    used_[hash % used_.size()] = true;
  }

  void Clear() {
    for (size_t i = 0; i < used_.size(); i++) used_[i] = false;
  }

  int FillP() {
    int count = 0;
    for (auto i : used_)
      if (i == true) count++;
    return (float)count / used_.size() * 100;
  }

  int FillC() {
    int count = 0;
    for (auto i : used_)
      if (i == true) count++;
    return count;
  }

 private:
  std::vector<T> table_;
  std::vector<bool> used_;
};

//template<typename T>
class TransPositionTable {
 public:
  TransPositionTable() {}

  std::optional<SearchElement> Search(bitboard_hash_t hash) {
    return search_table_.Find(hash);
  };
  void Write(bitboard_hash_t hash, const SearchElement &elem) {
    search_table_.Write(hash, elem);
  };
  int Fill() { return search_table_.FillP(); }

 private:
  BitBoardMap<SearchElement> search_table_;
  // BitBoardMap<QSearchElement> qsearch_table_;
};

#endif
