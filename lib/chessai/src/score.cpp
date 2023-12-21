#include "score.hpp"

#include "evaluator.hpp"

static Score::ProcessType kinvalid_score(
    std::numeric_limits<Score::ScoreType>::min());
static Score::ProcessType kmax_score(
    std::numeric_limits<Score::ScoreType>::max());
static Score::ProcessType kmin_score(
    -std::numeric_limits<Score::ScoreType>::max());
static Score::ProcessType kcheckmate_score(
    -std::numeric_limits<Score::ScoreType>::max() + 50);

Score::Score() : Score(kinvalid_score) {}

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
  Score value{kmin_score + depthmax - depthleft + 1};
  return value.IsCheckMate() ? value : kcheckmate_score;
}

Score Score::Tie() { return {0}; }

Score Score::GetStaticValue(const BitBoard &board, Color color,
                            const GameStage &settings) {
  Evaluator evaluator(board, color, settings);
  return Clamp(evaluator.Evaluate());
}

Score Score::Max() { return kmax_score; }

Score Score::Min() { return kmin_score; }

bool Score::IsValid() const { return *this != kinvalid_score; }

bool Score::IsCheckMate() const {
  return m_value <= kcheckmate_score || m_value >= -kcheckmate_score;
}

int Score::GetTurnsToCheckMate() const {
  if (m_value < 0)
    return m_value - kmin_score - 1;
  else
    return -kmin_score - m_value - 1;
}

Score Score::GetFigureScore(Figure figure) {
  return Evaluator::GetFigurePrice(figure);
}

std::string Score::ToCentiPawns() const {
  if (IsCheckMate())
    return std::string("M") + std::to_string(GetTurnsToCheckMate());
  else
    return std::to_string(m_value);
}

Score::operator ProcessType() { return m_value; }

Score::ScoreType Score::Clamp(ProcessType value) {
  if (value <= kcheckmate_score) return kcheckmate_score + 1;
  if (value >= -kcheckmate_score) return kcheckmate_score - 1;
  return value;
}
