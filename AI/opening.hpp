#ifndef OPENING_HPP
#define OPENING_HPP

#include <vector>

#include <turn.hpp>

class Opening{
public:
    Opening(std::vector<Turn> turns, std::string name):
        turns_(std::move(turns)),
        name_(std::move(name))
    {}
    std::string GetName() const {
        return name_;
    }
    std::vector<Turn> GetTurns() const {
        return turns_;
    }
private:
    std::vector<Turn> turns_;
    std::string name_;
};

#endif
