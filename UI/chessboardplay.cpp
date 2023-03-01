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

    for(auto && turn : turns_)
    {
        if(turn.from() == current_figure_)
            qp.drawEllipse(QRect(dx+(0.15+turn.to().y())*w_tile,dy+(0.15+turn.to().x())*h_tile,0.7*w_tile,0.7*h_tile));
    }
    /*
    for(size_t i = 0 ; i < 64 ; i++)
        if(board.UnderAtack(Position(i)))
            qp.drawEllipse(QRect(dx+(0.15+Position(i).y())*w_tile,dy+(0.15+Position(i).x())*h_tile,0.7*w_tile,0.7*h_tile));
            */
}

void ChessBoardPlay::mouseReleaseEvent(QMouseEvent *event)
{
    auto pos = event->pos();
    QPoint point((pos.x()-dx)/w_tile, (pos.y()-dy)/h_tile);
    if(point.x() >= 0 && point.x() < 8 && point.y() >= 0 && point.y() < 8)
    {
        auto position = Position(point.y() , point.x());
        if(!current_figure_.Valid())//figure is chosen
        {
            if(!board.TestEmp(position) &&( mode_ == TurnMode::OneSide ?board.TestColor(view,position): 1))
            {

                current_figure_ = position;
                turns_ = board.GenerateTurns();
            }
        }else {
            if(board.TestEmp(position)||!board.TestColor(board.GetColor(current_figure_),position))
            {
                board.ExecuteTurn(Turn(current_figure_,position));
                current_figure_ = Position();
                turns_.clear();
            } else {
                current_figure_ = position;
                turns_ = board.GenerateTurns();
            }
        }
    }
    repaint();
}

void ChessBoardPlay::SetMode(TurnMode mode)
{
    mode_ = mode;
}
