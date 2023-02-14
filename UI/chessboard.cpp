#include "chessboard.h"
#include <QPainter>
#include <QFrame>
#include <QRect>
#include <QBrush>
#include <QDebug>
#include <QResizeEvent>
#include <turngenerator.h>
#include <turnexecutor.h>

ChessBoardView::ChessBoardView(QWidget *parent) : QWidget(parent), view(white)
{
    pack.figures_texture[0][pawn].load("textures/WPawn.png");
    pack.figures_texture[0][knight].load("textures/WKnight.png");
    pack.figures_texture[0][bishop].load("textures/WBishop.png");
    pack.figures_texture[0][rook].load("textures/WRook.png");
    pack.figures_texture[0][queen].load("textures/WQueen.png");
    pack.figures_texture[0][king].load("textures/WKing.png");
    pack.figures_texture[1][pawn].load("textures/BPawn.png");
    pack.figures_texture[1][knight].load("textures/BKnight.png");
    pack.figures_texture[1][bishop].load("textures/BBishop.png");
    pack.figures_texture[1][rook].load("textures/BRook.png");
    pack.figures_texture[1][queen].load("textures/BQueen.png");
    pack.figures_texture[1][king].load("textures/BKing.png");
}

void ChessBoardView::resizeEvent(QResizeEvent* event) {
    auto width = size().width();
    auto height = size().height();

    auto less_side = std::min(width, height);

    dx = (width-less_side)/2 ;
    dy = (height-less_side)/2;

    w_tile = less_side/8;
    h_tile = less_side/8;
}

void ChessBoardView::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);

    qp.fillRect(QRect(dx,dy,w_tile*8,h_tile*8),QBrush(QColor(255,203,155)));

    bool is_white = false;
    for (size_t x = 0; x < 8; x++)
    {
        for (size_t y = 0; y < 8; y++)
        {
            is_white = !is_white;
            QColor back_color =  is_white ? QColor(255,203,155) : QColor(145,95,80);
            qp.fillRect(QRect(y*w_tile+dx,x*h_tile+dy,w_tile,h_tile),QBrush(back_color));

            auto position = Position(x,y);
            if(view == black)position = 63-position;

            auto figure = board.GetFigure(position);
            if(figure == empty)continue;

            auto color = board.GetColor(position);

            qp.drawImage(y*w_tile+dx,x*h_tile+dy,pack.figures_texture[color][figure].scaled(w_tile,h_tile,Qt::AspectRatioMode::IgnoreAspectRatio,Qt::TransformationMode::SmoothTransformation));
        }
        is_white = !is_white;
    }
}

void ChessBoardView::Upload( const Board &board)
{
    this->board = board;
    repaint();
}

void ChessBoardView::SetView(Color view)
{
   this->view = view;
    repaint();
}

Color ChessBoardView::View()
{
    return view;
}

QSize ChessBoardView::sizeHint() const
{
    return {600,600};
}
