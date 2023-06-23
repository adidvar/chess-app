#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include "statistics.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>

#include <match.hpp>
#include <position_rating.hpp>
#include <functional>

/*
struct callback_t
{
     std::function<void(size_t )> next_stage_done;
     std::function<void(size_t )> finished;
};
*/

class Computer{

    const Match &match_;
    const Color color_;

    std::vector<std::pair<Turn,MainAppraiser>> marks_;
    Statistics stat_;
    std::mutex marks_mutex_;

    std::condition_variable condition_;
    std::atomic<uint64_t> threads_working_flags_;
    std::atomic<bool> exit_flag_;

    std::vector<std::unique_ptr<std::thread>> thread_pool_;

    void Thread(size_t id, size_t all);

public:
    Computer(Match &match, Color color);
    ~Computer();

    void Start();

    void Wait();
    bool Ready();

    void LoadTurnsMarks(std::vector<std::pair<Turn,MainAppraiser>> &marks);

    Statistics GetStatistics();
};


#endif
