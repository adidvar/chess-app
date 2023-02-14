#include "gameui.h"
#include "ui_gameui.h"
#include <cmath>

GameUI::GameUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameUI)
{
    ui->setupUi(this);
    SetTimers(300,300);
    SetNames("Gitler","Stalin");
}

GameUI::~GameUI()
{
    delete ui;
}

void GameUI::SetTimers(int wtime, int btime)
{
    ui->wtimer_label->setText(QString("%1:%2").arg(wtime/60).arg(wtime%60));
    ui->btimer_label->setText(QString("%1:%2").arg(btime/60).arg(btime%60));
}
void GameUI::SetNames(QString wname, QString bname){
    ui->wname->setText(wname);
    ui->bname->setText(bname);
}
