#include "hmi.h"
#include "zcontrol.h"
#include "piinfo.h"

#include <QApplication>
#include <QFile>

// static PiInfo info;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/* -----------
    QFile f(":/sheets/dark");
    if(f.open(QFile::ReadOnly | QFile::Text))
    {
        a.setStyleSheet(f.readAll());
    }
 -------- */
    ZControl ctrl;
    Hmi w;
    w.show(); // showFullScreen();
    int ret = a.exec();

    return ret;
}
