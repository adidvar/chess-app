#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <match.hpp>

struct callback_t{};

class Computer{


public:
    void SetMatch(Match & match);
    void SetCallBack(callback_t callback);
    void Start();
    void Stop();

    void LoadTurnsMarks();
};


#endif
