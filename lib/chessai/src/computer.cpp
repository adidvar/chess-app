#include "computer.hpp"

Computer::Computer(Color color) : m_color(color) {}

Computer::~Computer() {}

void Computer::SetMatch(const Match &match) {}

void Computer::Start() {}

void Computer::Stop() {}

std::vector<Turn> Computer::GetPV() const { return m_pv; }

Evaluate Computer::GetValue() const { return m_value; }

Turn Computer::GetTurn() const { return m_turn; }

Statistics Computer::GetStatistics() { return m_stat; }
