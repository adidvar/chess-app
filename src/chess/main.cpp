#include <QApplication>
#include <QHBoxLayout>

#include "singleplayer.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  SinglePlayer single;
  single.show();
  return a.exec();
}
