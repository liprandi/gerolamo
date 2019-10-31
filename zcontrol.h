#ifndef ZCONTROL_H
#define ZCONTROL_H
#include "hx711.h"
#include "stepper.h"
#include "zconfig.h"


class ZControl
{
public:
    ZControl();
    ~ZControl();
private:
    void setupGpio();

public:
    static Hx711* g_hx711;
    static Stepper* g_stepper;
    static ZConfig* g_config;
};

#endif // ZCONTROL_H
