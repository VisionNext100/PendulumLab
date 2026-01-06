#include "pendulumwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PendulumWidget w;
    w.setWindowTitle("单摆和多摆物理模拟器");
    w.resize(1000, 600);
    w.show();

    return a.exec();
}
