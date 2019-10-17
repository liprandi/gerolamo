#include "hmi.h"

#include <QApplication>
#include <QDebug>
#include "piinfo.h"

static PiInfo info;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Hmi w;
    w.show();
    return a.exec();
}
