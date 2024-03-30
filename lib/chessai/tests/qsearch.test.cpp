#include "qsearch.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "evaluator.hpp"
#include "score.hpp"

/*
 *
  T qsearch(const BitBoardTuple &tuple, T alpha, T beta, int depth) {
    ExtraNode();

    CheckStopFlag();

    EndNode();
    T stand_pat = T::GetStaticValue(tuple.board, tuple.board.CurrentColor(),
                                    GetSearchSettings().GetStage());

    if (stand_pat >= beta) return stand_pat;

    if (stand_pat < alpha - kBIG_DELTA) return alpha;

    if (alpha < stand_pat) alpha = stand_pat;

    BitBoardTuple::GenerateTuplesFast(
        Get(depth), tuple, tuple.board.CurrentColor(), kall,
        tuple.board.GetColorBitBoard(tuple.board.OpponentColor()), false);
    auto &moves = Get(depth);

    if (moves.empty()) return stand_pat;

    QuiescenceSearchOrdering(tuple.board, moves);
    T bestscore = T::Min();
    for (auto &sub : moves) {
      auto score = -qsearch(sub, -beta, -alpha, depth + 1);
      if (score >= beta) {  // beta cutoff
        bestscore = score;
        break;
      }
      if (score > bestscore) {
        bestscore = score;
        if (score > alpha) {
          alpha = score;
        };
      }
    }

    return bestscore;
 */

/*
class QSearchTest {
 public:
  void Test() {
    // TO DO *****
    QSearch search(Color::kWhite);
    auto board = BitBoard{"startpos"};
    SECTION("Basic tests") {}
    SECTION("Stand pat tests") {}
    SECTION("Delta pruning tests") {}
    SECTION("Logic tests") {}
  }
};

TEST_CASE("QSearch tests", "[qsearch]", ) {
  QSearchTest test;
  test.Test();
}
*/
