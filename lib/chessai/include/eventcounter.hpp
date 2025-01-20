#pragma once

#include <atomic>

class EventCounter {
 public:
  EventCounter() = default;

  // no memory barriers
  void newPosition() { m_positions.fetch_add(1, std::memory_order_relaxed); }
  uint64_t getPosition() { return m_positions.load(std::memory_order_relaxed); }

 private:
  std::atomic<uint64_t> m_positions;
};

class StopFlag {
 public:
  StopFlag() = default;

  // no memory barriers
  void stop() { m_stop.store(true, std::memory_order_relaxed); }
  operator bool() { return m_stop.load(std::memory_order_relaxed); }

 private:
  std::atomic<bool> m_stop{false};
};
