#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <bitboard.hpp>

#include "BoardDesign.h"

class BoardWidget : public QWidget
{
    Q_OBJECT

    enum Mode{
        kPlayerWhite = Color::kWhite,
        kPlayerBlack = Color::kBlack,
        kPlayerTwoSides,
        kViewer,
    };

public:
    explicit BoardWidget(QWidget *parent = nullptr, Mode mode = kViewer, BitBoard board = BitBoard(), Turn last_turn = Turn());
    void PushTurn(Turn turn);
    void PushBoard(BitBoard board, Turn last_turn = Turn());

signals:
   void EnteredTurn(Turn);
   void EnteredBoard(BitBoard);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void RenderGrid(QPainter &event);
    void RenderFigures(QPainter &event);
    void RenderSelection(QPainter &event);
    void RenderLastMovePositions(QPainter &event , Turn last);
    void RenderFigureChoise(QPainter &event , Position position);

    virtual int heightForWidth(int w) const override;
    virtual bool hasHeightForWidth() const override;
    virtual QSize sizeHint() const override{return {500,500};};

private:
    BoardDesign design_;
    BitBoard board_;

    std::vector<Turn> possible_;
    Turn last_turn_;
    Position selected_;
    Position hovered_;
    QPoint hovered_p_;
    Turn turn_;
    bool tracking_ = false;
    bool choise_mode_ = false;

    bool animation_enabled = false;
    float animation_progress_ = 0;
    Q_PROPERTY(float animation_progress MEMBER animation_progress_ NOTIFY animation_state_updated);

    Mode mode_;

    bool Move(Turn turn);
    bool IsPawnTransform(Turn turn) const;
    Position ToPosition(QPoint point) const ;
    QRect GenerateFigureChoisePosition(Position pos) const;

private slots:
   void update_animation_frame();
   void animation_finished();

signals:
   void animation_state_updated();
};

#endif // CHESSBOARD_H
