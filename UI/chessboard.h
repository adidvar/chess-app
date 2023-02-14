#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <board.h>
#include <unordered_map>
#include <QPicture>
#include <QMouseEvent>
#include <turn.h>

struct texture_pack
{
    std::unordered_map<Figures,QImage> figures_texture[2];
};

class ChessBoardView : public QWidget
{
    Q_OBJECT
protected:
    Board board;
    Color view;
    texture_pack pack;

    size_t dx,dy;
    size_t w_tile,h_tile;

public:
    explicit ChessBoardView(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void Upload(const Board &board);

    void SetView(Color view);
    Color View();
public:
    virtual QSize sizeHint() const override;
signals:

};

#endif // CHESSBOARD_H
