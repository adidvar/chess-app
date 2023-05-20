#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <chrono>

class IStatistics{
public:
    using duration = std::chrono::high_resolution_clock::duration;
    using point = std::chrono::high_resolution_clock::time_point;
    using clock = std::chrono::high_resolution_clock;

    virtual void NewNodeEvent() = 0;
    virtual void NewApproximationEvent() = 0;

    virtual void AllTimerBegin() = 0;
    virtual void AllTimerEnd() = 0;
    virtual void ApproximationTimerBegin() = 0;
    virtual void ApproximationTimerEnd() = 0;
    virtual void GenerationTimerBegin() = 0;
    virtual void GenerationTimerEnd() = 0;
};

class Statistics final : public IStatistics{
public:
    Statistics()
    {
    }

    virtual void NewNodeEvent() override
    {
        nodescount_++;
    };

    virtual void NewApproximationEvent() override
    {
        approximation_++;
    };

    virtual void AllTimerBegin() override
    {
       allp_ = clock::now();
    };

    virtual void AllTimerEnd() override
    {
       all_ += clock::now() - allp_;
    };

    virtual void ApproximationTimerBegin() override
    {
       approximationp_ = clock::now();
    };

    virtual void ApproximationTimerEnd() override
    {
        approximation_ += clock::now() - approximationp_;
    };

    virtual void GenerationTimerBegin() override
    {
        generationp_ = clock::now();
    };
    virtual void GenerationTimerEnd() override
    {
        generation_ += clock::now() - generationp_;
    };

    duration GetAllTime() const {
        return all_;
    }
    duration GetApproximationTime() const {
        return approximation_;
    }
    duration GetGenerationTime() const {
        return generation_;
    }
    int GetNodesCount() const {
        return nodescount_;
    }
    int GetApproximationCount() const {
        return approximationcount_;
    }

private:
    int nodescount_;
    int approximationcount_;

    duration all_;
    duration approximation_;
    duration generation_;

    point allp_;
    point approximationp_;
    point generationp_;
};

class NoStatistics final: public IStatistics{
    virtual void NewNodeEvent() override
    { };
    virtual void NewApproximationEvent() override
    { };
    virtual void AllTimerBegin() override
    { };
    virtual void AllTimerEnd() override
    { };
    virtual void ApproximationTimerBegin() override
    { };
    virtual void ApproximationTimerEnd() override
    { };
    virtual void GenerationTimerBegin() override
    { };
    virtual void GenerationTimerEnd() override
    { };
};

#endif
