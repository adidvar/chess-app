#include <QApplication>
#include <QHBoxLayout>
#include "BoardWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    QHBoxLayout layout;

    BoardWidget b1;
    BoardWidget b2;
    BoardWidget b3;
    layout.addWidget(&b1);
    layout.addWidget(&b2);
    layout.addWidget(&b3);
    w.setLayout(&layout);

    w.show();
    return a.exec();
}
