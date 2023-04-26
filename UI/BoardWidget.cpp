#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>

static float borderwidth = 2;

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
Position BoardWidget::ToPosition(QPoint point) const
{
    auto size = this->size();
    float rectSizex = (size.width()-2*borderwidth) / 8.0;
    float rectSizey = (size.height()-2*borderwidth) / 8.0;

    float p_x = point.x() , p_y = point.y();

    if(p_x < borderwidth || p_x > size.width() - borderwidth ||
       p_y < borderwidth || p_y > size.height() - borderwidth  )
        return Position();

    p_x -= borderwidth;
    p_y -= borderwidth;

    return Position(p_y/rectSizex ,p_x/rectSizey);
}

BoardWidget::BoardWidget(QWidget *parent):
QWidget(parent)
{
    possible_ = board_.GenerateTurns();
}

void BoardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    RenderGrid(painter);
    RenderFigures(painter);
    if(chosen_pos_.Valid())
        RenderPossibleMoves(painter,chosen_pos_);
}

void BoardWidget::mouseReleaseEvent(QMouseEvent *event)
{
    chosen_pos_ = ToPosition(event->pos());
    repaint();
}

void BoardWidget::RenderGrid(QPainter &qp)
{
    auto size = this->size();

    qp.fillRect(QRect(0,0,size.width(),size.height()),QBrush(design_.GetFill()));

    float rectSizex = (size.width()-2*borderwidth) / 8.0;
    float rectSizey = (size.height()-2*borderwidth) / 8.0;

    //draw grid
    bool is_white = false;
    for (size_t y = 0; y < 8; y++)
    {
        for (size_t x = 0; x < 8; x++)
        {
            is_white = !is_white;
            QColor back_color =  is_white ? design_.GetBack() : design_.GetFront();
            qp.fillRect(QRect(borderwidth+x*rectSizex,borderwidth+y*rectSizey,ceil(rectSizex),ceil(rectSizey)),QBrush(back_color));
        }
        is_white = !is_white;
    }
    //draw a-h 1-8
    QFont serifFont = design_.GetFont();
    serifFont.setPixelSize(rectSizex/6);

    qp.setFont(serifFont);
    is_white = true;
    if(rectSizex/6 > 10){
        for (size_t x = 0; x < 8; x++)
        {
            QColor back_color =  is_white ? design_.GetBack() : design_.GetFront();
            qp.setPen(QPen(back_color));
            qp.drawText(QPoint(borderwidth+rectSizex/20+x*rectSizex,borderwidth-rectSizey/20+8*rectSizey),QString(char('a'+x)));
            qp.drawText(QPoint(borderwidth-rectSizex/7+8*rectSizex,borderwidth+rectSizey/4+x*rectSizey),QString(char('8'-x)));
            is_white = !is_white;
        }
    }
}

void BoardWidget::RenderFigures(QPainter &qp)
{
    auto size = this->size();
    float rectSizex = (size.width()-2*borderwidth) / 8.0;
    float rectSizey = (size.height()-2*borderwidth) / 8.0;

    for (size_t x = 0; x < 8; x++)
    {
        for (size_t y = 0; y < 8; y++)
        {
            auto cell = board_.GetCell(Position(y,7-x));

            if(cell.type == Figure::kEmpty)continue;

            QRect render_rect(borderwidth+x*rectSizex,borderwidth+y*rectSizey,ceil(rectSizex),ceil(rectSizey));
            qp.drawPixmap(render_rect,design_.GetTexture(),design_.GetRenderRect(cell.type,cell.color));
        }
    }
}

void BoardWidget::RenderPossibleMoves(QPainter &qp, Position position)
{
    auto size = this->size();
    float rectSizex = (size.width()-2*borderwidth) / 8.0;
    float rectSizey = (size.height()-2*borderwidth) / 8.0;

    for(auto move : possible_){
        if(move.from() == position){
            int x = move.to().y() , y = move.to().x();
            qp.setBrush(QBrush(QColor(0,220,0)));
            qp.setPen(QPen(Qt::NoPen));
            float rx = rectSizex/4 , ry = rectSizey/4;
            qp.drawEllipse(borderwidth+x*rectSizex + rectSizex/2 - rx/2 ,borderwidth+y*rectSizey + rectSizey/2 - ry/2 ,rx,ry);
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
