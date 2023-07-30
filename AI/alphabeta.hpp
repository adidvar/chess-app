#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include "transpositiontable.hpp"
#include <bitboard.hpp>
#include <statistics.hpp>

#include <iostream>
#include <algorithm>


#include <iostream>
using namespace std;

template<typename T>
inline void ReOrder(const BitBoard &board, T begin, T end)
{

    std::sort(begin, end, [&](const BitBoardTuple & t1, const BitBoardTuple & t2) {

        const static uint8_t order[] = {
            0,
            1,
            30,
            30,
            50,
            90,
            0
        };

        if (board.TestEmp(t1.turn.to()) != board.TestEmp(t2.turn.to()))
            return !board.TestEmp(t1.turn.to()) < !board.TestEmp(t2.turn.to());
        else if (!board.TestEmp(t1.turn.to()) && !board.TestEmp(t2.turn.to()))
            if (order[board.GetFigure(t1.turn.to())] != order[board.GetFigure(t2.turn.to())])
                return order[board.GetFigure(t1.turn.to())] < order[board.GetFigure(t2.turn.to())];
            else
                return order[board.GetFigure(t1.turn.from())] < order[board.GetFigure(t2.turn.from())];
        else
            return order[board.GetFigure(t1.turn.from())] < order[board.GetFigure(t2.turn.from())];

    });
}

template<typename T>
class AlphaBeta
{
    std::pair<T, Turn> quiescence(const BitBoardTuple &tuple, T a, T b) {
      stat_.ExtraNode();

      auto stand_pat = T::Approximate(tuple.board, color_);
      if (stand_pat >= b) return {b, Turn()};

      int big_delta = 900;  // queen value

      if (stand_pat < a - big_delta) {
        return {a, Turn()};
      }

      if (a < stand_pat) a = stand_pat;

      auto nodes = tuple.board.GenerateTuplesFast(
          tuple, tuple.board.CurrentColor(), kall,
          tuple.board.GetColorBitBoard(tuple.board.OpponentColor()));

      if (nodes.size() == 0) return {stand_pat, Turn()};

      ReOrder(tuple.board, nodes.rbegin(), nodes.rend());

      T value = T::Invalid();
      Turn best_turn;

      if (tuple.board.CurrentColor() == color_) {
        value = T::Min();
        for (auto &node : nodes) {
          auto nvalue = quiescence(node, a, b).first;
          if (nvalue > value) {
            value = nvalue;
            best_turn = node.turn;
          }

          a = std::max(a, value);
          if (b <= a) break;
        }
      } else {
        value = T::Max();
        for (auto &node : nodes) {
          auto nvalue = quiescence(node, a, b).first;
          if (nvalue < value) {
            value = nvalue;
            best_turn = node.turn;
          }

          b = std::min(b, value);
          if (b <= a) break;
        }
      }

      return {value, best_turn};
    }

    std::pair<T, Turn> alphabeta(const BitBoardTuple &tuple, const int depth,
                                 const int max_depth, T a, T b) {
      auto hashed = table_.Search(tuple.hash);
      if (hashed.has_value() && hashed->hash == tuple.hash) {
        /*
        if (hashed->value.first <= hashed->a) b = std::min(b, hashed->a);
        if (hashed->value.first >= hashed->b) a = std::max(a, hashed->b);
*/
        if (hashed->value.first > hashed->a &&
            hashed->value.first < hashed->b && hashed->depth >= depth)
          return hashed->value;
      }

      /*
      if (depth == 1) {
        auto stand_pat = T::Approximate(tuple.board, color_);
        if (stand_pat >= b) return {b, Turn()};

        int big_delta = 900;  // queen value

        if (stand_pat < a - big_delta) {
          return {a, Turn()};
        }
      }
*/

      auto nodes =
          tuple.board.GenerateTuplesFast(tuple, tuple.board.CurrentColor());

      if (nodes.size() == 0)
        if (!tuple.board.MateTest())
          return {T::Tie(), Turn()};
        else
          return {tuple.board.CurrentColor() == color_
                      ? T::CheckMateLose(max_depth - depth)
                      : T::CheckMateWin(max_depth - depth),
                  Turn()};
      else if (depth == 0) {
        auto approx = T::Approximate(tuple.board, color_);
        auto static_eval = quiescence(tuple, std::max(a, approx), b);
        // return {approx, Turn()};
        return static_eval;
      }
      stat_.MainNode();
      if (hashed.has_value() && tuple.board.OpponentColor() == color_) {
        auto best_hashed = hashed.value().value.second;

        for (size_t i = 0; i < nodes.size(); ++i)
          if (nodes[i].turn == best_hashed) {
            std::swap(nodes[i], nodes[0]);
            break;
          }

        ReOrder(tuple.board, nodes.rbegin(), nodes.rend() - 1);
      } else
        ReOrder(tuple.board, nodes.rbegin(), nodes.rend());

      T value = T::Invalid();
      Turn best_turn;

      if (tuple.board.CurrentColor() == color_) {
        value = T::Min();
        for (auto &node : nodes) {
          auto nvalue = alphabeta(node, depth - 1, max_depth, a, b).first;
          if (nvalue > value) {
            value = nvalue;
            best_turn = node.turn;
          }

          a = std::max(a, value);
          if (b <= a) break;
        }
      } else {
        value = T::Max();
        for (auto &node : nodes) {
          auto nvalue = alphabeta(node, depth - 1, max_depth, a, b).first;
          if (nvalue < value) {
            value = nvalue;
            best_turn = node.turn;
          }

          b = std::min(b, value);
          if (b <= a) break;
        }
      }

      if (!hashed.has_value() ||
          (hashed->depth < depth && hashed->hash == tuple.hash) ||
          (hashed->hash != tuple.hash)) {
        SearchElement elem;
        elem.hash = tuple.hash;
        elem.value = {value, best_turn};
        elem.depth = depth;
        elem.a = a;
        elem.b = b;
        table_.Write(tuple.hash, elem);
      }

      return {value, best_turn};
    }

   public:
    AlphaBeta(Color color, Statistics &stat, TransPositionTable &table):
        color_(color),
        stat_(stat),
        table_(table)
    {
    }

    T GetValue(const BitBoard &board, int depth, T a = T::Min(),
               T b = T::Max()) {
      stat_.Clear();
      return alphabeta({board, board.Hash(), Turn()}, depth, depth, a, b).first;
    }

    Turn GetBestTurn(const BitBoard &board, int depth) {
      stat_.Clear();
      return alphabeta({board, board.Hash(), Turn()}, depth, depth, T::Min(),
                       T::Max())
          .second;
    }

    static T Evaluate(BitBoard board, Color color, int depth, Statistics &stat )
    {
        TransPositionTable table;
        AlphaBeta<T> core(color, stat, table);
        return core.GetValue(board, depth);
    }
private:

    Color color_;
    Statistics &stat_;
    TransPositionTable &table_;
};

#endif
