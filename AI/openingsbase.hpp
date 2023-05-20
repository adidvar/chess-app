#ifndef OPENINGBASE_HPP
#define OPENINGBASE_HPP

#include <opening.hpp>

class OpeningBase{
public:
    OpeningBase()
    {}

    Turn GetNextTurn();
private:
    std::vector<Opening> openings_;
};

#endif
