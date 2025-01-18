#pragma once

struct SearchSettings {
  bool enable_alpha_beta = true;

  bool enable_qsearch = true;
  bool enable_stand_pat = true;
  bool enable_delta_pruning = true;

  bool enable_transposition = true;

  bool enable_ordering_pv = true;
  bool enable_ordering_hashed = true;
  bool enable_ordering_killer = true;
  bool enable_ordering_history = true;

  bool enable_null_move_pruning_2 = false;
  bool enable_null_move_pruning_3 = false;
};
