#include <QApplication>
#include <QHBoxLayout>

#include "mainmenu.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainMenu widget;
  widget.show();
  return a.exec();
}
