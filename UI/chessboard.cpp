#include "chessboard.h"
#include <QPainter>
#include <QFrame>
#include <QRect>
#include <QBrush>
#include <QDebug>
#include <QResizeEvent>

ChessBoardView::ChessBoardView(QWidget *parent) : QWidget(parent), view(Color::kWhite)
{
    pack.figures_texture[0][Figure::kPawn].load("textures/WPawn.png");
    pack.figures_texture[0][Figure::kKnight].load("textures/WKnight.png");
    pack.figures_texture[0][Figure::kBishop].load("textures/WBishop.png");
    pack.figures_texture[0][Figure::kRook].load("textures/WRook.png");
    pack.figures_texture[0][Figure::kQueen].load("textures/WQueen.png");
    pack.figures_texture[0][Figure::kKing].load("textures/WKing.png");
    pack.figures_texture[1][Figure::kPawn].load("textures/BPawn.png");
    pack.figures_texture[1][Figure::kKnight].load("textures/BKnight.png");
    pack.figures_texture[1][Figure::kBishop].load("textures/BBishop.png");
    pack.figures_texture[1][Figure::kRook].load("textures/BRook.png");
    pack.figures_texture[1][Figure::kQueen].load("textures/BQueen.png");
    pack.figures_texture[1][Figure::kKing].load("textures/BKing.png");
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

            size_t position = Position(x,y).Value();
            if(view == Color::kBlack)position = 63-position;

            auto figure = board.GetFigure(position);
            qp.drawText(y*w_tile+dx,x*h_tile+h_tile/2+dy,QString::number(position));
            if(figure == Figure::kEmpty)continue;

            auto color = board.GetColor(position);
            auto texture = pack.figures_texture[(uint8_t)color][figure].scaled(w_tile,h_tile,Qt::AspectRatioMode::IgnoreAspectRatio,Qt::TransformationMode::SmoothTransformation);
            qp.drawImage(y*w_tile+dx,x*h_tile+dy,texture);
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
