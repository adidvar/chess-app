#include <alphabeta.hpp>
#include <chrono>
#include <computer.hpp>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>

int main() {
  /*
  std::atomic_bool flag = 0;
  ItDeepening<Evaluate> ab(Color::kWhite, flag);
  TTable table;
  ab.SetTTable(&table);
  std::thread thread([&flag]() {
    std::this_thread::sleep_for(std::chrono::milliseconds{10000});
    flag = 1;
  });
  auto pv = ab.FindPV(BitBoard(), 10);
  for (auto eleme : pv) std::cout << eleme.ToChessFormat() << " ";
  thread.join();
*/
  BitBoard board{
      "rnb1kbnr/pppppppp/8/2q5/1P3P2/4B2N/P1PPP1PP/RN1QKB1R w KQkq - 0 1"};

  auto turns = BitBoard::GenerateTuplesFast({board, board.Hash(), Turn(), 0},
                                            Color::kWhite);

  TTable table;

  auto begin = std::chrono::high_resolution_clock::now();

  ItDeepening ab(Color::kWhite);
  ab.SetTTable(&table);
  std::cout << ab.GetValue(board, 8).ToCentiPawns() << std::endl;
  std::cout << "time: "
            << (std::chrono::duration_cast<
                    std::chrono::duration<float, std::ratio<1, 1>>>(
                    std::chrono::high_resolution_clock::now() - begin)
                    .count())
            << std::endl;
  std::cout << ab.GetTurn(board, 8).ToChessFormat() << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time: "
            << (std::chrono::duration_cast<
                    std::chrono::duration<float, std::ratio<1, 1>>>(end - begin)
                    .count())
            << std::endl;

  BFTable btable;
  ReOrder(board, turns, Evaluate::Min(), Evaluate::Max(), btable, &table, 8);

  for (auto turn : turns) {
    std::cout << turn.turn.ToChessFormat() << " <---> " << turn.priority.type
              << " <---> " << turn.priority.index << "<--->";
    bool founded = false;
    auto elem = table.Search(turn.hash, founded);
    std::cout << (founded ? elem->value : Evaluate::Min()).ToString()
              << std::endl;
  }
  bool founded = false;
  auto elem = table.Search(board.Hash(), founded);
  std::cout << elem->value.ToString() << " <---> " << elem->a.ToString()
            << " <---> " << elem->b.ToString() << " <---> " << std::endl;
}
