#ifndef BOARDDESIGN_H
#define BOARDDESIGN_H
#include <QPixmap>
#include <QFont>

#include <figures.hpp>

class BoardDesign{
    QFont font_;
    QPixmap pixmap_;

public:
    BoardDesign();

    QColor GetFront() const;
    QColor GetBack() const;
    QColor GetFill() const;
    QColor GetFillMate() const;
    QColor GetMoveColor() const;
    QColor GetLastMoveColor() const;
    QColor GetFigureChoiseBackground() const;
    QColor GetFigureChoiseSelected() const;

    QRect GetRenderRect(Figure figure, Color color) const ;
    const QPixmap &GetTexture() const ;
    const QFont &GetFont() const ;
};

#endif
