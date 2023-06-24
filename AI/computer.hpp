#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include "statistics.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>

#include <match.hpp>
#include <position_rating.hpp>
#include <functional>
#include <openingsbase.hpp>

#include <transpositiontable.hpp>

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

    TransPositionTable table_;

    Statistics stat_;
public:
    Computer(Match &match, Color color);
    ~Computer();

    void Start();

    void Wait();
    bool Ready();

    Turn GetBestTurn();

    Statistics GetStatistics();
};


#endif
