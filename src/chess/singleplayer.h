#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H

#include <QMainWindow>
#include <QTimer>
#include <computer.hpp>

#include "match.hpp"

namespace Ui {
class SinglePlayer;
}

class SinglePlayer : public QMainWindow
{
    Q_OBJECT
    Match match;
    ThreadController *computer_;
    TTable table_;

   public:
    explicit SinglePlayer(QWidget *parent = nullptr);
    ~SinglePlayer();

   public slots:
    void ComputerEntered();

   private:
    Ui::SinglePlayer *ui;

   public slots:
    void TurnEntered(Turn);
};

#endif // SINGLEPLAYER_H
