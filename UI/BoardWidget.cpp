#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>


static float borderwidth = 2;

bool BoardWidget::Move(Turn turn, bool fromTracking)
{
    if(std::count(possible_.begin(),possible_.end(),turn) == 1 && ( mode_ == board_.CurrentColor() || mode_ == kPlayerTwoSides))
    {
        qDebug() << turn.ToChessFormat().c_str();
        last_turn_ = turn;

        animation_enabled = true;
        animation_progress_ = 0;
        QPropertyAnimation *anim = new QPropertyAnimation(this, "animation_progress", this);
        connect(anim,&QPropertyAnimation::finished,this,&BoardWidget::animation_finished);
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::InOutCubic);
        anim->setStartValue(0);
        anim->setEndValue(1);
        anim->start();

        return true;
    }
    else {
        return false;
    }
}

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
    auto pos = Position(p_x/rectSizex ,p_y/rectSizey);
    if(mode_ == kPlayerBlack)
        pos = pos.Rotate();
    return pos;
}

BoardWidget::BoardWidget(QWidget *parent):
QWidget(parent),
  board_("rnbqkbnr/pppppppp/8/8/8/8/PP2PPPP/RNBQKBNR w KQkq - 0 1")
{
    connect(this,&BoardWidget::animation_state_updated,this,&BoardWidget::update_animation_frame);
    setMouseTracking(true);
    possible_ = board_.GenerateTurns();
}

void BoardWidget::update_animation_frame()
{
    repaint();
}

void BoardWidget::animation_finished()
{
    animation_enabled = false;
    board_.ExecuteTurn(last_turn_);

    possible_ = board_.GenerateTurns( mode_ != kPlayerTwoSides ? board_.OpponentColor() : board_.CurrentColor());
    repaint();
    emit TurnDone(last_turn_);
}


void BoardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    RenderGrid(painter);
    RenderSelection(painter);
    RenderFigures(painter);
}

void BoardWidget::mouseReleaseEvent(QMouseEvent *event)
{
    auto pos = ToPosition(event->pos());

    if(selected_.Valid() && mode_ != kViewer && Move(Turn(selected_,pos),true) ){
        selected_ = Position();
    }

    tracking = false;
    repaint();
}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    tracking = true;
    auto pos = ToPosition(event->pos());

    if(selected_.Valid() && mode_ != kViewer && Move(Turn(selected_,pos),false)){
        selected_ = Position();
    }
    else if(pos != selected_)
        selected_ = pos;
    else
        selected_ = Position();

    repaint();
}

void BoardWidget::mouseMoveEvent(QMouseEvent *event)
{
    hovered_p_ = event->pos();
    hovered_ = ToPosition(hovered_p_);
    repaint();
}

void BoardWidget::leaveEvent(QEvent *event)
{
}

void BoardWidget::RenderGrid(QPainter &qp)
{
    auto size = this->size();

    auto back_color = board_.MateTest() ? design_.GetFillMate() : design_.GetFill();
    qp.fillRect(QRect(0,0,size.width(),size.height()),QBrush(back_color));


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
            char num;
            char letter;
            if(mode_ == kPlayerBlack)
            {
                num = '1'+x;
                letter = 'h'-x;
            }
            else
            {
                num = '8'-x;
                letter = 'a'+x;
            }
            qp.drawText(QPoint(borderwidth+rectSizex/20+x*rectSizex,borderwidth-rectSizey/20+8*rectSizey),QString(letter));
            qp.drawText(QPoint(borderwidth-rectSizex/7+8*rectSizex,borderwidth+rectSizey/4+x*rectSizey),QString(num));
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
            auto position = Position(x,y);
            if(mode_ == kPlayerBlack)
                position = position.Rotate();
            auto cell = board_.GetCell(position);

            if(position == last_turn_.from() && animation_progress_) continue;

            if(cell.type == Figure::kEmpty)continue;

            if(position == selected_ && tracking){
                qp.setOpacity(0.5);
            }
            else {
                qp.setOpacity(1);
            }

            QRect render_rect(borderwidth+x*rectSizex,borderwidth+y*rectSizey,ceil(rectSizex),ceil(rectSizey));
            qp.drawPixmap(render_rect,design_.GetTexture(),design_.GetRenderRect(cell.type,cell.color));
        }
    }
    if(last_turn_.Valid() & animation_enabled){
        float x = last_turn_.from().x()*rectSizex * (1-animation_progress_) + last_turn_.to().x()*rectSizex * animation_progress_;
        float y = last_turn_.from().y()*rectSizey * (1-animation_progress_) + last_turn_.to().y()*rectSizey * animation_progress_;

        if(mode_ == Color::kBlack) {
            x = (rectSizex*7-x);
            y = (rectSizey*7-y);
        }

        auto cell = board_.GetCell(last_turn_.from());
        QRect render_rect(borderwidth+x,borderwidth+y,ceil(rectSizex),ceil(rectSizey));
        qp.drawPixmap(render_rect,design_.GetTexture(),design_.GetRenderRect(cell.type,cell.color));
    }


    if(hovered_.Valid() && selected_.Valid() && tracking && !board_.TestEmp(selected_))
    {
        auto cell = board_.GetCell(selected_);
        QRect render_rect(borderwidth+hovered_p_.x()-rectSizex/2,borderwidth+hovered_p_.y()-rectSizey/2,ceil(rectSizex),ceil(rectSizey));
        qp.drawPixmap(render_rect,design_.GetTexture(),design_.GetRenderRect(cell.type,cell.color));
    }
}

void BoardWidget::RenderSelection(QPainter &qp )
{
    auto size = this->size();
    float rectSizex = (size.width()-2*borderwidth) / 8.0;
    float rectSizey = (size.height()-2*borderwidth) / 8.0;
    //moves
    for(auto move : possible_){
        if(move.from() == selected_){
            if(move.to() != hovered_)
            {
                int x = move.to().x() , y = move.to().y();
                if(mode_ == Color::kBlack)
                {
                    x = 7-x;
                    y = 7-y;
                }
                qp.setBrush(QBrush(design_.GetMoveColor()));
                qp.setPen(QPen(Qt::NoPen));
                float rx = rectSizex/3 , ry = rectSizey/3;
                qp.drawEllipse(borderwidth+x*rectSizex + rectSizex/2 - rx/2 ,borderwidth+y*rectSizey + rectSizey/2 - ry/2 ,rx,ry);
            } else {
                auto x = hovered_.x() , y= hovered_.y();
                if(mode_ == Color::kBlack)
                {
                    x = 7-x;
                    y = 7-y;
                }
                qp.fillRect(QRect(borderwidth+x*rectSizex,borderwidth+y*rectSizey,ceil(rectSizex),ceil(rectSizey)),QBrush(design_.GetMoveColor()));
            }

        }
    }
    //selected
    if(selected_.Valid())
    {
        auto x = selected_.x() , y = selected_.y();
        if(mode_ == Color::kBlack) {
            x = 7-x;
            y = 7-y;
        }
        qp.fillRect(QRect(borderwidth+x*rectSizex,borderwidth+y*rectSizey,ceil(rectSizex),ceil(rectSizey)),QBrush(design_.GetMoveColor()));
    }
    //last turn
    if(last_turn_.Valid())
    {
        auto x = last_turn_.from().x() , y = last_turn_.from().y();
        if(mode_ == Color::kBlack) {
            x = 7-x;
            y = 7-y;
        }
        qp.fillRect(QRect(borderwidth+x*rectSizex,borderwidth+y*rectSizey,ceil(rectSizex),ceil(rectSizey)),QBrush(design_.GetLastMoveColor()));
        x = last_turn_.to().x() , y = last_turn_.to().y();
        if(mode_ == Color::kBlack) {
            x = 7-x;
            y = 7-y;
        }
        qp.fillRect(QRect(borderwidth+x*rectSizex,borderwidth+y*rectSizey,ceil(rectSizex),ceil(rectSizey)),QBrush(design_.GetLastMoveColor()));
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
