#ifndef STATISTICS_HPP
#define STATISTICS_HPP

class Statistics {
 public:
  void MainNode() { generation_count_++; };
  void EndNode() { approximation_count_++; };
  void ExtraNode() { extra_node_++; };

  [[nodiscard]] int GetMainNode() const { return generation_count_; };
  [[nodiscard]] int GetEndNode() const { return approximation_count_; };
  [[nodiscard]] int GetExtraNode() const { return extra_node_; };

  void operator+=(const Statistics &stat) {
    generation_count_ += stat.generation_count_;
    approximation_count_ += stat.approximation_count_;
    extra_node_ += stat.extra_node_;
  };

  void Clear() {
    generation_count_ = 0;
    approximation_count_ = 0;
    extra_node_ = 0;
  }

 private:
  int generation_count_ = 0;
  int approximation_count_ = 0;
  int extra_node_ = 0;
};

#endif
