#ifndef STATISTICS_HPP
#define STATISTICS_HPP

class Statistics
{
public:
    void Generation(){
        generation_count_++;
    };
    void Approximation(){
        approximation_count_++;
    };
    void ExtraNode(){
        extra_node_++;
    };

    int GetGeneration(){
        return generation_count_;
    };
    int GetApproximation(){
        return approximation_count_;
    };
    int GetExtraNode(){
        return extra_node_;
    };

    void operator += (const Statistics &stat){
        generation_count_ += stat.generation_count_;
        approximation_count_ += stat.approximation_count_;
        extra_node_ += stat.extra_node_;
    };

private:
    int generation_count_ = 0;
    int approximation_count_ = 0;
    int extra_node_ = 0;
};

#endif
