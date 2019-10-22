#include "hmi.h"
#include "zcontrol.h"
#include "piinfo.h"

#include <QApplication>
#include <QDebug>

// static PiInfo info;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZControl ctrl;
    Hmi w;
    w.show(); // showFullScreen();
    int ret = a.exec();

    return ret;
}
