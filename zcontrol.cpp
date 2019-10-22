#include "zcontrol.h"
#include <wiringPi.h>

Hx711* ZControl::g_hx711 = nullptr;

ZControl::ZControl()
{
    if (wiringPiSetup () < 0)
        exit(-1);
    setupGpio();
    g_hx711 = new Hx711(4, 14, 15, 18);

}
ZControl::~ZControl()
{
    if(g_hx711)
        delete g_hx711;
    g_hx711 = nullptr;
}

void ZControl::setupGpio()
{
}
