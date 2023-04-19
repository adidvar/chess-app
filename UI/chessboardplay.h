#ifndef CHESSBOARDPLAY_H
#define CHESSBOARDPLAY_H

#include <vector>

#include <QVector>
#include <QMouseEvent>

#include "chessboard.h"
#include "turn.hpp"
#include <figures.hpp>
#include <computer.hpp>

class ChessBoardPlay : public ChessBoardView
{
    enum class TurnMode{
        OneSide,
        TwoSide,
        FreeMove
    } mode_ = TurnMode::TwoSide;

    Q_OBJECT
    Position current_figure_;
    Computer cmp;
    bool underline_moves_ = true;
    std::vector<Turn> turns_ = {};
public:
    explicit ChessBoardPlay(QWidget *parent = nullptr);
    void Upload(const Board &board);
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void EnableMoveUnderline(bool mode);
    void SetMode(TurnMode mode);
signals:
    void NewTurn(Position);
};

#endif // CHESSBOARDPLAY_H
