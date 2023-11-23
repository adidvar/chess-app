#include <alphabeta.hpp>
#include <chrono>
#include <computer.hpp>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>

#include "pvs.hpp"

int main() {
  std::string fen{"startpos"};
  // std::getline(std::cin, fen);
  BitBoard board(fen);
  auto value = Score::Value(board, Color::kWhite);
  std::cout << value.ToCentiPawns() << std::endl;

  TTable table;
  {
    std::string fen{"startpos"};
    int d = 8;
    ItDeepening<AlphaBeta> cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);
    std::cout << cmp.GetValue({fen}, d).ToCentiPawns() << std::endl;
    std::cout << cmp.GetStatistics().GetMainNode() << std::endl;

    std::cout << cmp.GetTurn({fen}, d).ToChessFormat() << std::endl;
    std::cout << cmp.GetStatistics().GetMainNode() << std::endl;

    auto pv = cmp.FindPV({fen}, d);
    for (auto p : pv) std::cout << p.ToChessFormat() << " ";

    std::cout << std::endl;
    std::cout << cmp.GetStatistics().GetMainNode() << std::endl;
  }
  std::cout << "----------------------" << std::endl;
  {
    std::string fen{"startpos"};
    int d = 7;
    ItDeepening<AlphaBeta> cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);
    std::cout << cmp.GetValue({fen}, d).ToCentiPawns() << std::endl;
    std::cout << cmp.GetStatistics().GetMainNode() << std::endl;

    std::cout << cmp.GetTurn({fen}, d).ToChessFormat() << std::endl;
    std::cout << cmp.GetStatistics().GetMainNode() << std::endl;

    auto pv = cmp.FindPV({fen}, d);
    for (auto p : pv) std::cout << p.ToChessFormat() << " ";

    std::cout << std::endl;
    std::cout << cmp.GetStatistics().GetMainNode() << std::endl;
  }
  std::cout << "----------------------" << std::endl;
  {
    TTable table;
    std::string fen{"startpos"};
    int d = 7;
    ItDeepening<PVS> cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);
    std::cout << cmp.GetValue({fen}, d).ToCentiPawns() << std::endl;
    std::cout << cmp.GetStatistics().GetMainNode() << std::endl;

    std::cout << cmp.GetTurn({fen}, d).ToChessFormat() << std::endl;
    std::cout << cmp.GetStatistics().GetMainNode() << std::endl;

    auto pv = cmp.FindPV({fen}, d);
    for (auto p : pv) std::cout << p.ToChessFormat() << " ";

    std::cout << std::endl;
    std::cout << cmp.GetStatistics().GetMainNode() << std::endl;
  }
}
