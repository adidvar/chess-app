#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
 //   void resizeEvent(QResizeEvent *event) override;
public:
  //  virtual QSize sizeHint() const override;
    virtual int heightForWidth(int w) const override;
    virtual bool hasHeightForWidth() const override;
    virtual QSize sizeHint() const override{return {100,100};};

signals:

};

#endif // CHESSBOARD_H
