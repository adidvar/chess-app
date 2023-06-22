#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H

#include <QMainWindow>
#include <computer.hpp>

namespace Ui {
class SinglePlayer;
}

class SinglePlayer : public QMainWindow
{
    Q_OBJECT
    Match match;
    Computer computer_;
public:
    explicit SinglePlayer(QWidget *parent = nullptr);
    ~SinglePlayer();

private:
    Ui::SinglePlayer *ui;

public slots:
    void TurnEntered(Turn);
};

#endif // SINGLEPLAYER_H
