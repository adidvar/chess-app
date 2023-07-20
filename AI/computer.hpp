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

class Computer{
    Match match_;

    Statistics stat_;
public:
    Computer();
    ~Computer();

    void Position(const Match &match);
    void NewGame();

    void Go();

    void Wait();
    bool Ready();

    Turn GetBestTurn();

    Statistics GetStatistics();

};


#endif
