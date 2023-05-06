#include "aiwidget.h"
#include "ui_aiwidget.h"

AIWidget::AIWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AIWidget)
{
    ui->setupUi(this);
}

AIWidget::~AIWidget()
{
    delete ui;
}

void AIWidget::on_set_clicked()
{
     home = BitBoard(ui->fen->text().toStdString());
     ui->widget->PushBoard(home);
}

