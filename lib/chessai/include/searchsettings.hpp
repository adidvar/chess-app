#pragma once

class SearchSettings {
 public:
  [[nodiscard]] bool GetAlphaBetaEnabled() const;
  void SetAlphaBetaEnabled(bool /*Enable_alpha_beta*/);

  [[nodiscard]] bool GetQSearchEnabled() const;
  void SetQSearchEnabled(bool /*Enable_qsearch*/);

  [[nodiscard]] bool GetStandPatEnabled() const;
  void SetStandPatEnabled(bool /*Enable_stand_pat*/);

  [[nodiscard]] bool GetDeltaPruningEnabled() const;
  void SetDeltaPruningEnabled(bool /*Enable_delta_pruning*/);

  [[nodiscard]] bool GetTranspositionEnabled() const;
  void SetTranspositionEnabled(bool /*Enable_transposition*/);

  [[nodiscard]] bool GetOrderingPVEnabled() const;
  void SetOrderingPVEnabled(bool /*Enable_ordering_pv*/);

  [[nodiscard]] bool GetOrderingHashedEnabled() const;
  void SetOrderingHashedEnabled(bool /*Enable_ordering_hashed*/);

  [[nodiscard]] bool GetOrderingKillerEnabled() const;
  void SetOrderingKillerEnabled(bool /*Enable_ordering_killer*/);

  [[nodiscard]] bool GetOrderingHistoryEnabled() const;
  void SetOrderingHistoryEnabled(bool /*Enable_ordering_history*/);

  [[nodiscard]] bool GetNullMovePruining2Enabled() const;
  void SetNullMovePruining2Enabled(bool /*Enable_null_move_pruning_2*/);

  [[nodiscard]] bool GetNullMovePruning3Enabled() const;
  void SetNullMovePruning3Enabled(bool /*Enable_null_move_pruning_3*/);

  [[nodiscard]] unsigned int GetGameStageSlider() const;
  void SetGameStageSlider(unsigned int /*Game_stage_slider*/);

 private:
  bool m_enable_alpha_beta = true;

  bool m_enable_qsearch = true;
  bool m_enable_stand_pat = true;
  bool m_enable_delta_pruning = true;

  bool m_enable_transposition = true;

  bool m_enable_ordering_pv = true;
  bool m_enable_ordering_hashed = true;
  bool m_enable_ordering_killer = true;
  bool m_enable_ordering_history = true;

  bool m_enable_null_move_pruning_2 = false;
  bool m_enable_null_move_pruning_3 = false;

  unsigned m_game_stage_slider = 0;  //< [0;100]
};

inline bool SearchSettings::GetAlphaBetaEnabled() const {
  return m_enable_alpha_beta;
}

inline void SearchSettings::SetAlphaBetaEnabled(bool Enable_alpha_beta) {
  m_enable_alpha_beta = Enable_alpha_beta;
}

inline bool SearchSettings::GetQSearchEnabled() const {
  return m_enable_qsearch;
}

inline void SearchSettings::SetQSearchEnabled(bool Enable_qsearch) {
  m_enable_qsearch = Enable_qsearch;
}

inline bool SearchSettings::GetStandPatEnabled() const {
  return m_enable_stand_pat;
}

inline void SearchSettings::SetStandPatEnabled(bool Enable_stand_pat) {
  m_enable_stand_pat = Enable_stand_pat;
}

inline bool SearchSettings::GetDeltaPruningEnabled() const {
  return m_enable_delta_pruning;
}

inline void SearchSettings::SetDeltaPruningEnabled(bool Enable_delta_pruning) {
  m_enable_delta_pruning = Enable_delta_pruning;
}

inline bool SearchSettings::GetTranspositionEnabled() const {
  return m_enable_transposition;
}

inline void SearchSettings::SetTranspositionEnabled(bool Enable_transposition) {
  m_enable_transposition = Enable_transposition;
}

inline bool SearchSettings::GetOrderingPVEnabled() const {
  return m_enable_ordering_pv;
}

inline void SearchSettings::SetOrderingPVEnabled(bool Enable_ordering_pv) {
  m_enable_ordering_pv = Enable_ordering_pv;
}

inline bool SearchSettings::GetOrderingHashedEnabled() const {
  return m_enable_ordering_hashed;
}

inline void SearchSettings::SetOrderingHashedEnabled(
    bool Enable_ordering_hashed) {
  m_enable_ordering_hashed = Enable_ordering_hashed;
}

inline bool SearchSettings::GetOrderingKillerEnabled() const {
  return m_enable_ordering_killer;
}

inline void SearchSettings::SetOrderingKillerEnabled(
    bool Enable_ordering_killer) {
  m_enable_ordering_killer = Enable_ordering_killer;
}

inline bool SearchSettings::GetOrderingHistoryEnabled() const {
  return m_enable_ordering_history;
}

inline void SearchSettings::SetOrderingHistoryEnabled(
    bool Enable_ordering_history) {
  m_enable_ordering_history = Enable_ordering_history;
}

inline bool SearchSettings::GetNullMovePruining2Enabled() const {
  return m_enable_null_move_pruning_2;
}

inline void SearchSettings::SetNullMovePruining2Enabled(
    bool Enable_null_move_pruning_2) {
  m_enable_null_move_pruning_2 = Enable_null_move_pruning_2;
}

inline bool SearchSettings::GetNullMovePruning3Enabled() const {
  return m_enable_null_move_pruning_3;
}

inline void SearchSettings::SetNullMovePruning3Enabled(
    bool Enable_null_move_pruning_3) {
  m_enable_null_move_pruning_3 = Enable_null_move_pruning_3;
}

inline unsigned int SearchSettings::GetGameStageSlider() const {
  return m_game_stage_slider;
}

inline void SearchSettings::SetGameStageSlider(unsigned int Game_stage_slider) {
  m_game_stage_slider = Game_stage_slider;
}
