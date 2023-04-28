#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <bitboard.hpp>

#include "BoardDesign.h"

class BoardWidget : public QWidget
{
    Q_OBJECT
    BoardDesign design_;
    BitBoard board_;

    std::vector<Turn> possible_;
    Turn last_turn_;
    Position selected_;
    Position hovered_;
    QPoint hovered_p_;
    bool tracking = false;

    bool animation_enabled = false;
    float animation_progress_ = 0;
    Q_PROPERTY(float animation_progress MEMBER animation_progress_ NOTIFY animation_state_updated);

    enum Mode{
        kPlayerWhite = Color::kWhite,
        kPlayerBlack = Color::kBlack,
        kPlayerTwoSides,
        kViewer,
    } mode_ = kPlayerTwoSides;

    bool Move(Turn turn, bool fromTracking);
    Position ToPosition(QPoint point) const ;
public:
    explicit BoardWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

    void RenderGrid(QPainter &event);
    void RenderFigures(QPainter &event);
    void RenderSelection(QPainter &event);
    void RenderLastMovePositions(QPainter &event , Turn last);

    virtual int heightForWidth(int w) const override;
    virtual bool hasHeightForWidth() const override;
    virtual QSize sizeHint() const override{return {500,500};};

signals:
   void animation_state_updated();
   void TurnDone(Turn);
public slots:
   void update_animation_frame();
   void animation_finished();

};

#endif // CHESSBOARD_H
