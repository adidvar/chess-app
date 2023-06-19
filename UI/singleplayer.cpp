#include "singleplayer.h"
#include "ui_singleplayer.h"

SinglePlayer::SinglePlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SinglePlayer)
{
    ui->setupUi(this);
}

SinglePlayer::~SinglePlayer()
{
    delete ui;
}
