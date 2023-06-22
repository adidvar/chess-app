#include "computer.hpp"
#include "alphabeta.hpp"

void Computer::Thread(size_t id, size_t all)
{
    while (!exit_flag_) {

        //waiting loop
        {
            std::unique_lock guard(marks_mutex_);
            while ((threads_working_flags_ & (1ULL << id)) == 0)
                condition_.wait(guard);
        }

        //task
        {
            std::unique_lock guard(marks_mutex_);
            for(size_t i = id ; i < marks_.size() ; i+= all)
            {
                auto board = match_.GetBoard();
                board.ExecuteTurn(marks_[i].first);
                Statistics stat;
                AlphaBeta<MainAppraiser> evaluator(color_,stat);
                guard.unlock();
                auto value = evaluator.Evaluate(board,5);
                guard.lock();
                stat_ += stat;
                marks_[i].second = value;
            }
        }

        //done flag
        threads_working_flags_ &= ~(1ULL << id);
    }
}

Computer::Computer(Match &match, Color color):
    match_(match),
    color_(color)
{
    exit_flag_ = 0;
    size_t concurrency = std::min<size_t>(64,std::thread::hardware_concurrency());
    for(size_t i = 0 ; i < concurrency ; ++i)
        thread_pool_.emplace_back(new std::thread(std::bind(&Computer::Thread,this,i,concurrency)));
}

Computer::~Computer()
{
    exit_flag_ = 1;
    while ( thread_pool_.size() )
    {
        if(thread_pool_.back()->joinable())
            thread_pool_.back()->join();
        thread_pool_.pop_back();
    }
}

void Computer::Start()
{
    std::lock_guard guard(marks_mutex_);
    marks_.clear();

    assert(match_.GetBoard().CurrentColor() == color_);
    for(auto &turn : match_.GetBoard().GenerateTurns(color_))
        marks_.emplace_back(turn,MainAppraiser::Invalid());

    threads_working_flags_ = (1ULL << thread_pool_.size())-1;
    condition_.notify_all();
}

void Computer::Wait()
{
    while (threads_working_flags_)
        std::this_thread::yield();
}

bool Computer::Ready()
{
    return threads_working_flags_ == 0;
}

void Computer::LoadTurnsMarks(std::vector<std::pair<Turn, MainAppraiser> > &marks)
{
    std::lock_guard guard(marks_mutex_);
    marks = marks_;

}
