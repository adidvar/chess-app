#include "score.hpp"

#include <algorithm>

#include "evaluator.hpp"

static Score::ScoreType k_invalid =
    std::numeric_limits<Score::ScoreType>::min();
static Score::ScoreType k_min = k_invalid + 1;
static Score::ScoreType k_checkmate_0 = k_min + 1;
static Score::ScoreType k_checkmate_max = k_checkmate_0 + 100;

Score::Score() : Score(k_invalid) {}

Score::Score(ProcessType type) : m_value(type) {}

bool Score::operator<(Score value) const { return m_value < value.m_value; }

bool Score::operator>(Score value) const { return m_value > value.m_value; }

bool Score::operator<=(Score value) const { return m_value <= value.m_value; }

bool Score::operator>=(Score value) const { return m_value >= value.m_value; }

bool Score::operator==(Score value) const { return m_value == value.m_value; }

bool Score::operator!=(Score value) const { return m_value != value.m_value; }

Score Score::operator-() const { return -this->m_value; }

Score Score::operator-(Score value) const {
  return Clamp(this->m_value - value.m_value);
}

Score Score::operator+(Score value) const {
  return Clamp(this->m_value + value.m_value);
}

Score Score::operator*(int value) const { return Clamp(this->m_value * value); }

Score Score::CheckMate(int depthleft, int depthmax) {
  Score::ProcessType value{k_checkmate_0 + depthmax - depthleft};
  return std::min<Score::ProcessType>(k_checkmate_max, value);
}

Score Score::Tie() { return {0}; }

Score Score::GetStaticValue(const BitBoard &board, Color color,
                            const GameStage &settings) {
  Evaluator evaluator(board, color, settings);
  return Clamp(evaluator.Evaluate());
}

Score Score::Max() { return -k_min; }

Score Score::Min() { return k_min; }

bool Score::IsValid() const { return *this != k_invalid; }

bool Score::IsCheckMate() const {
  return m_value <= k_checkmate_max || m_value >= -k_checkmate_max;
}

int Score::GetTurnsToCheckMate() const {
  if (m_value < 0)
    return m_value - k_checkmate_0;
  else
    return -m_value - k_checkmate_0;
}

Score Score::GetFigureScore(Figure figure) {
  return Evaluator::GetFigurePrice(figure);
}

std::string Score::ToCentiPawns() const {
  if (*this == Score{})
    return "I";
  else if (IsCheckMate())
    return std::string("M") + std::to_string(GetTurnsToCheckMate());
  else
    return std::to_string(m_value);
}

Score::operator ProcessType() { return m_value; }

Score::ScoreType Score::Clamp(ProcessType value) {
  if (value <= k_checkmate_max) return k_checkmate_max + 1;
  if (value >= -k_checkmate_max) return -k_checkmate_max - 1;
  return value;
}
