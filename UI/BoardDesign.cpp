#include "BoardDesign.h"

BoardDesign::BoardDesign():
    font_("Times",10,QFont::Bold),
    pixmap_("chessfigures.png")
{
}

QColor BoardDesign::GetFront() const
{
    return QColor(145,95,80);
}

QColor BoardDesign::GetBack() const
{
    return QColor(255,203,155);
}

QColor BoardDesign::GetFill() const
{
    return QColor(0,0,0);
}

QColor BoardDesign::GetFillMate() const
{
    return QColor(0,255,255);
}

QRect BoardDesign::GetRenderRect(Figure figure, Color color) const
{
    int delta_x = pixmap_.width()/6 , delta_y = pixmap_.height()/2;

    int texture_y  = delta_y * (color == Color::kWhite ? 1 : 0);

    int map_rect[7] {
        0,5,4,3,2,0,1
    };

    int texture_x  = delta_x * map_rect[figure];

    return {texture_x,texture_y,delta_x,delta_y};
}

const QPixmap &BoardDesign::GetTexture() const
{
    return pixmap_;
}

const QFont &BoardDesign::GetFont() const
{
    return font_;
}
