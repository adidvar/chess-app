#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool {
  using Task = std::function<void()>;

 public:
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;

  ThreadPool(size_t threads = std::thread::hardware_concurrency()) {
    for (size_t i = 0; i < threads; ++i) {
      m_threads.emplace_back([this] {
        // main loop
        while (true) {
          Task task;
          {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_cv.wait(lock, [this] { return !m_tasks.empty() || m_stop; });

            if (m_stop && m_tasks.empty()) {
              return;
            }

            task = std::move(m_tasks.front());
            m_tasks.pop();
          }

          task();
        }
      });
    }
  }

  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_stop = true;
    }

    m_cv.notify_all();

    for (auto& thread : m_threads) {
      thread.join();
    }
  }

  template <typename Lambda>
  void enqueue(Lambda task) {
    {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_tasks.emplace(task);
    }
    m_cv.notify_one();
  }

 private:
  std::vector<std::thread> m_threads;
  std::queue<Task> m_tasks;

  std::mutex m_mutex;
  std::condition_variable m_cv;
  bool m_stop = false;
};
