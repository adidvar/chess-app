#ifndef CHESSBOARDPLAY_H
#define CHESSBOARDPLAY_H

#include "chessboard.h"
#include "turn.h"
#include <QVector>
#include <QMouseEvent>

class ChessBoardPlay : public ChessBoardView
{
    Q_OBJECT
    Position current_figure;
    bool underline_moves = true;
    QVector<Position> turns = {};
public:
    explicit ChessBoardPlay(QWidget *parent = nullptr);
    void Upload(const Board &board);
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void EnableMoveUnderline(bool mode);
signals:
    void NewTurn(Position);
};

#endif // CHESSBOARDPLAY_H
