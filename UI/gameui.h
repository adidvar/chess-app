#ifndef GAMEUI_H
#define GAMEUI_H

#include <QMainWindow>

namespace Ui {
class GameUI;
}

class GameUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameUI(QWidget *parent = nullptr);
    ~GameUI();

private:
    Ui::GameUI *ui;

    void SetTimers(int wtime, int btime);
    void SetNames(QString wname, QString bname);

};

#endif // GAMEUI_H
