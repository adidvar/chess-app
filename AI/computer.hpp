#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <turn.hpp>
#include <bitboard.hpp>

class Computer
{
protected:
    Color color_;
public:
    Computer(Color color){
        color_ = color;
    };
    virtual float Evaluate(BitBoard board);
};

#endif
