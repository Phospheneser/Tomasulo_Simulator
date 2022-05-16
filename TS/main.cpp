#include "Widget.h"
#include "modular.h"
#include<fstream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    w.sim.mem[34].fv = 1;
    w.sim.mem[45].fv = 2;

    return a.exec();
}
