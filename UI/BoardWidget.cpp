#include "BoardWidget.h"
#include <QPainter>

/*
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
*/
BoardWidget::BoardWidget(QWidget *parent):
QWidget(parent)
{
}

void BoardWidget::paintEvent(QPaintEvent *event)
{

    QPainter qp(this);
    auto size = this->size();

    float borderwidth = 2;

    qp.fillRect(QRect(0,0,size.width(),size.height()),QBrush(QColor(0,0,0)));

    float rectSizex = (size.width()-2*borderwidth) / 8.0;
    float rectSizey = (size.height()-2*borderwidth) / 8.0;

    //draw grid
    bool is_white = false;
    for (size_t y = 0; y < 8; y++)
    {
        for (size_t x = 0; x < 8; x++)
        {
            is_white = !is_white;
            QColor back_color =  is_white ? QColor(255,203,155) : QColor(145,95,80);
            qp.fillRect(QRect(borderwidth+x*rectSizex,borderwidth+y*rectSizey,ceil(rectSizex),ceil(rectSizey)),QBrush(back_color));
        }
        is_white = !is_white;
    }
    //draw a-h 1-8
    QFont serifFont("Times", rectSizex/6, QFont::Bold);
    qp.setFont(serifFont);
    is_white = true;
    if(rectSizex > 30){
        for (size_t x = 0; x < 8; x++)
        {
            QColor back_color =  is_white ? QColor(255,203,155) : QColor(145,95,80);
            qp.setPen(QPen(back_color));
            qp.drawText(QPoint(borderwidth+rectSizex/20+x*rectSizex,borderwidth-rectSizey/20+8*rectSizey),QString(char('a'+x)));
            qp.drawText(QPoint(borderwidth-rectSizex/7+8*rectSizex,borderwidth+rectSizey/4+x*rectSizey),QString(char('1'+x)));
            is_white = !is_white;
        }
    }
}

int BoardWidget::heightForWidth(int w) const
{
    return w;
}

bool BoardWidget::hasHeightForWidth() const
{
    return true;
}
