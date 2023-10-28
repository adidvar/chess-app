#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>


static float borderwidth = 2;
const Figure figure_placing[4] = {Figure::kKnight, Figure::kBishop,
                                  Figure::kRook, Figure::kQueen};

BoardWidget::BoardWidget(QWidget *parent, Mode mode, BitBoard board,
                         Turn last_turn)
    : QWidget(parent), mode_(mode), board_(board), last_turn_(last_turn) {
  connect(this, &BoardWidget::animation_state_updated, this,
          &BoardWidget::update_animation_frame);
  setMouseTracking(true);
  Color color;
  if (mode_ == kPlayerBlack || mode_ == kPlayerWhite) {
    possible_ = board_.GenerateTurns(mode_);
  } else {
    possible_ = board_.GenerateTurns(board.CurrentColor());
  }
}

void BoardWidget::PushTurn(Turn turn)
{
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
}

void BoardWidget::PushBoard(BitBoard board, Turn last_turn)
{
    //todo rewrite ***
    board_ = board;
    last_turn_ = last_turn;
    if(mode_ == kPlayerBlack || mode_ == kPlayerWhite){
        possible_ = board_.GenerateTurns(mode_);
    } else {
        possible_ = board_.GenerateTurns(board.CurrentColor());
    }
    repaint();
}

void BoardWidget::SetMode(Mode mode)
{
    mode_ = mode;
}

void BoardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    RenderGrid(painter);
    if(!choise_mode_)
    RenderSelection(painter);
    RenderFigures(painter);
    if(choise_mode_)
        RenderFigureChoise(painter,turn_.to());
}

void BoardWidget::mouseReleaseEvent(QMouseEvent *event)
{
    auto pos = ToPosition(event->pos());

    turn_.from() = selected_;
    turn_.to() = pos;
    turn_.figure() = Figure::kEmpty;

    if(selected_.Valid() && mode_ != kViewer && Move(turn_) ){
        selected_ = Position();
    }
    else if(selected_.Valid() && mode_ != kViewer && IsPawnTransform(turn_)){
        choise_mode_ = true;
    }

    tracking_ = false;
    repaint();
}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{

    auto size = this->size();

    float rectSizex = (size.width()-2*borderwidth) / 8.0;

    if(choise_mode_){
        auto rect = GenerateFigureChoisePosition(turn_.to());
        if(!rect.contains(event->pos()))
        {
            choise_mode_ = false;
            return;
        }
        Figure figure = figure_placing[int((event->pos().x() - rect.x() - borderwidth) /rectSizex)];
        turn_.figure() = figure;
        Move(turn_);
        choise_mode_ = false;
        return;
    };

    tracking_ = true;

    auto pos = ToPosition(event->pos());

    turn_.from() = selected_;
    turn_.to() = pos;
    turn_.figure() = Figure::kEmpty;

    if(selected_.Valid() && mode_ != kViewer && Move(turn_)){
        selected_ = Position();
        turn_ = Turn();
    }
    else if(selected_.Valid() && mode_ != kViewer && IsPawnTransform(turn_)){
        choise_mode_ = true;
    }
    else if(pos == selected_)
        selected_ = Position();
    else
        selected_ = pos;

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
    if(choise_mode_)
        choise_mode_ = false;
    if(tracking_)
        tracking_ = false;
    if(selected_.Valid())
        selected_ = Position();
    repaint();
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

            if(position == selected_ && tracking_){
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


    if(hovered_.Valid() && selected_.Valid() && tracking_ && !board_.TestEmp(selected_))
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

void BoardWidget::RenderFigureChoise(QPainter &qp, Position position)
{
    auto size = this->size();
    float rectSizex = (size.width()-2*borderwidth) / 8.0;
    float rectSizey = (size.height()-2*borderwidth) / 8.0;

    auto rect = GenerateFigureChoisePosition(position);

    qp.setPen(Qt::NoPen);
    qp.setBrush(QBrush(design_.GetFill()));
    qp.drawRect(rect.x(),rect.y(),rect.width(),rect.height());
    qp.setPen(Qt::NoPen);
    qp.setBrush(QBrush(design_.GetFigureChoiseBackground()));
    qp.drawRect(rect.x()+borderwidth,rect.y()+borderwidth,rect.width()-2*borderwidth,rect.height()-2*borderwidth);


    for(size_t x = 0 ; x < 4 ; x++)
    {
        QRect render_rect(borderwidth+rect.x()+x*rectSizex,borderwidth+rect.y(),ceil(rectSizex),ceil(rectSizey));
        if(render_rect.contains(hovered_p_)){
            qp.setPen(Qt::NoPen);
            qp.setBrush(QBrush(design_.GetFigureChoiseSelected()));
            qp.drawRect(borderwidth+rect.x()+x*rectSizex,borderwidth+rect.y(),ceil(rectSizex),ceil(rectSizey));
        }
        qp.drawPixmap(render_rect,design_.GetTexture(),design_.GetRenderRect(figure_placing[x],board_.CurrentColor()));
    }
}

bool BoardWidget::Move(Turn turn)
{
    if(std::count(possible_.begin(),possible_.end(),turn) == 1 && ( mode_ == board_.CurrentColor() || mode_ == kPlayerTwoSides))
    {
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

bool BoardWidget::IsPawnTransform(Turn turn) const
{
    turn.figure() = Figure::kQueen;
    if((turn.to().y() == 7 || turn.to().y() == 0) &&
        board_.Test(Figure::kPawn,turn.from()) && std::count(possible_.begin(),possible_.end(),turn) == 1)
        return true;
    else
        return false;
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

QRect BoardWidget::GenerateFigureChoisePosition(Position pos) const
{

    auto size = this->size();
    float rectSizex = (size.width()-2*borderwidth) / 8.0;
    float rectSizey = (size.height()-2*borderwidth) / 8.0;

    int dx = pos.x()*rectSizex;
    int dy = (pos.y() < 4 ? 1 : -1)*rectSizey*5/4 + pos.y()*rectSizey;
    int sx = ceil(4*rectSizex + 2*borderwidth) , sy = ceil(rectSizey + 2*borderwidth);

    if(pos.x() >= 4)
        dx -= sx - borderwidth*2;

    return {dx,dy,sx,sy};
}

void BoardWidget::update_animation_frame()
{
    repaint();
}

void BoardWidget::animation_finished()
{
    animation_enabled = false;
    board_.ExecuteTurn(last_turn_);

    Color color;
    if(mode_ == kPlayerTwoSides || mode_ == kViewer)
        color = board_.CurrentColor();
    else
        color = mode_;

    possible_ = board_.GenerateTurns( color );
    if(mode_ == board_.CurrentColor())
        return;
    repaint();
    emit EnteredTurn(last_turn_);
    emit EnteredBoard(board_);
}


int BoardWidget::heightForWidth(int w) const
{
    return w;
}

bool BoardWidget::hasHeightForWidth() const
{
    return true;
}
