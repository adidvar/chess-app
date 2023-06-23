#ifndef OPENINGBASE_HPP
#define OPENINGBASE_HPP

#include <opening.hpp>
#include <match.hpp>

class OpeningsBase{

public:
    OpeningsBase();

    std::vector<Turn> FindNext(const Match& match) const;
private:
    std::vector<Opening> openings_;
};

#endif
