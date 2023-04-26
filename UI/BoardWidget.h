#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <bitboard.hpp>

#include "BoardDesign.h"

class BoardWidget : public QWidget
{
    Q_OBJECT
    BoardDesign design_;
    BitBoard board_;
    Position ToPosition(QPoint point) const ;
public:
    explicit BoardWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void RenderGrid(QPainter &event);
    void RenderFigures(QPainter &event);

    virtual int heightForWidth(int w) const override;
    virtual bool hasHeightForWidth() const override;
    virtual QSize sizeHint() const override{return {500,500};};

signals:

};

#endif // CHESSBOARD_H
