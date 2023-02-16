#include "chessboardplay.h"
#include <QPainter>

ChessBoardPlay::ChessBoardPlay(QWidget *parent) : ChessBoardView(parent)
{
}

void ChessBoardPlay::Upload(const Board &board)
{

};

void ChessBoardPlay::paintEvent(QPaintEvent *event)
{
    ChessBoardView::paintEvent(event);

    QPainter qp(this);
    qp.setBrush(QBrush(QColor(80,150,250)));
    qp.setPen(QPen());

    qp.drawEllipse(QRect(dx+(0.15+current_figure.y())*w_tile,dy+(0.15+current_figure.x())*h_tile,0.7*w_tile,0.7*h_tile));
}

void ChessBoardPlay::mouseReleaseEvent(QMouseEvent *event)
{
    auto pos = event->pos();
    QPoint point((pos.x()-dx)/w_tile, (pos.y()-dy)/h_tile);
    if(point.x() >= 0 && point.x() < 8 && point.y() >= 0 && point.y() < 8)
    {
        auto position = Position(point.y() , point.x());
        if(current_figure == -1)
        {
            if(!board.TestEmp(position)&&board.TestColor(view,position))
                current_figure = position;
        }else {
            if(board.TestEmp(position)||!board.TestColor(view,position))
            {
                //move testing and generation
            } else {
                current_figure = -1;
            }
        }
    }
    repaint();
}
