#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <board.hpp>
#include <unordered_map>
#include <QPicture>
#include <QMouseEvent>
#include <turn.hpp>

namespace std {
  template <> struct hash<Figure>
  {
    size_t operator()(const Figure & x) const
    {
      return hash<uint8_t>()(x);
    }
  };
}

struct texture_pack
{
    std::unordered_map<Figure,QImage> figures_texture[2];
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
