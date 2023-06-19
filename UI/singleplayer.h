#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H

#include <QMainWindow>

namespace Ui {
class SinglePlayer;
}

class SinglePlayer : public QMainWindow
{
    Q_OBJECT
public:
    explicit SinglePlayer(QWidget *parent = nullptr);
    ~SinglePlayer();

private:
    Ui::SinglePlayer *ui;
};

#endif // SINGLEPLAYER_H
