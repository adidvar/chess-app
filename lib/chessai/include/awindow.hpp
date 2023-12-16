#ifndef AWINDOW_HPP
#define AWINDOW_HPP

#include "evaluate.hpp"

class AspirationWindow {
 public:
  std::pair<long int, long int> Forecast(std::vector<Score> vect,
                                         bool trend_up) {
    long int average = 0, dispersion = 0, linear = 0;

    if (vect.size() == 0) return {Score::Min().Value(), Score::Max().Value()};

    while (vect.size() > 3) vect.erase(vect.begin());

    for (auto value : vect) average += value.Value();
    average /= vect.size();
    for (auto value : vect)
      dispersion += (value.Value() - average) * (value.Value() - average);
    dispersion /= vect.size();
    linear = sqrt(dispersion);

    if (linear < 10) linear = 10;

    std::pair<long int, long int> result{average - 2 * linear,
                                         average + 2 * linear};

    if (trend_up)
      result.second = average + linear * 12;
    else
      result.first = average - linear * 12;

    return result;
  }

  void Push(Score score) {
    if (m_values_high.size() == m_values_low.size()) {
      m_values_high.push_back(score);
      auto forecast = Forecast(m_values_low, true);
      m_low = Score{forecast.first};
      m_high = Score{forecast.second};
    } else {
      m_values_low.push_back(score);
      auto forecast = Forecast(m_values_high, false);
      m_low = Score{forecast.first};
      m_high = Score{forecast.second};
    }
  };
  Score GetLow() const { return m_low; };
  Score GetHigh() const { return m_high; };

  void End();

 private:
  std::vector<Score> m_values_low;
  std::vector<Score> m_values_high;
  Score m_low = Score::Min();
  Score m_high = Score::Max();
};

#endif
