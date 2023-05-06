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
    std::unique_ptr<BitBoard> home;
    //BitBoard current;
public:
    explicit AIWidget(QWidget *parent = nullptr);
    ~AIWidget();

private slots:
    void on_set_clicked();

private:
    Ui::AIWidget *ui;
};

#endif // AIWIDGET_H
