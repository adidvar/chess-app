#include <QApplication>
#include <QHBoxLayout>
#include "aiwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AIWidget *widget = new AIWidget();
    widget->show();
    return a.exec();
}
