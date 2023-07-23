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
  int depth;
};

struct QSearchElement {
  bitboard_hash_t hash;
  std::pair<MainAppraiser, Turn> value = {MainAppraiser::Invalid(), Turn()};
};

template <class T>
class BitBoardMap {
 public:
  BitBoardMap(size_t size) : ksize_(size), table_(size), used_(size) {
    Clear();
  }

  std::optional<T> Find(bitboard_hash_t hash) {
    if (used_[hash % ksize_] == false)
      return {};
    else
      return table_[hash % ksize_];
  }
  void Write(bitboard_hash_t hash, const T &element) {
    table_[hash % ksize_] = element;
    used_[hash % ksize_] = true;
  }

  void Clear() {
    for (size_t i = 0; i < ksize_; i++) used_[i] = false;
  }

  int Count() {
    int count = 0;
    for (auto i : used_) count += i;
    return count;
  }

 private:
  const size_t ksize_;
  std::vector<T> table_;
  std::vector<bool> used_;
};

//template<typename T>
class TransPositionTable {
 public:
  TransPositionTable(size_t size = 64000)
      : search_table_(size), qsearch_table_(size) {}

  std::optional<QSearchElement> QSearch(bitboard_hash_t hash) {
    auto elem = qsearch_table_.Find(hash);
    if (elem.has_value()) {
      if (elem.value().hash == hash)
        return elem;
      else
        return {};
    }
    return {};
  };
  void QWrite(bitboard_hash_t hash, const QSearchElement &elem) {
    qsearch_table_.Write(hash, elem);
  };
  std::optional<SearchElement> Search(bitboard_hash_t hash, int depth) {
    auto elem = search_table_.Find(hash);
    if (elem.has_value()) {
      if (elem.value().hash == hash && elem->depth >= depth)
        return elem;
      else
        return {};
    }
    return {};
  };
  void Write(bitboard_hash_t hash, const SearchElement &elem) {
    search_table_.Write(hash, elem);
  };

 private:
  BitBoardMap<SearchElement> search_table_;
  BitBoardMap<QSearchElement> qsearch_table_;
};

#endif
