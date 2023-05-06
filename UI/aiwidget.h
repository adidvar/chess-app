#ifndef AIWIDGET_H
#define AIWIDGET_H

#include <QWidget>
#include <bitboard.hpp>
#include <memory>

namespace Ui {
class AIWidget;
}

class AIWidget : public QWidget
{
    Q_OBJECT
    BitBoard home;
public:
    explicit AIWidget(QWidget *parent = nullptr);
    ~AIWidget();

private slots:
    void on_set_clicked();

private:
    Ui::AIWidget *ui;
    //alignas(16) char array[256];
    //alignas(16) char array1[256];
};

#endif // AIWIDGET_H
