#include <QApplication>
#include "gameui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameUI w;

    w.show();
    return a.exec();
}
