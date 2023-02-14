#include "matchvisual.h"
#include <QPainter>
#include <QPainterPath>

MatchVisual::MatchVisual(QWidget *parent) : QWidget(parent)
{

}

void MatchVisual::paintEvent(QPaintEvent *event)

{
    QPainter qp(this);

  //  qp.drawpol

    for(auto x : this->data)
    {
    //    path.add
    }

}
