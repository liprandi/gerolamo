#include "zcontrol.h"
#include <wiringPi.h>

Hx711* ZControl::g_hx711 = nullptr;
Stepper* ZControl::g_stepper = nullptr;
ZConfig* ZControl::g_config = nullptr;

ZControl::ZControl()
{
    if (wiringPiSetup () < 0)
        exit(-1);
    setupGpio();
    g_config = new ZConfig;
    if(g_config)
    {
        g_hx711 = new Hx711(g_config->m_hx711.gpioclk, g_config->m_hx711.gpiodata1, g_config->m_hx711.gpiodata2, g_config->m_hx711.gpiodata3);
        g_stepper = new Stepper(g_config->m_stepper.gpiodir,
                                g_config->m_stepper.gpiopulse,
                                g_config->m_stepper.gpioenable,
                                g_config->m_stepper.gpiofault,
                                g_config->m_stepper.gpioforward,
                                g_config->m_stepper.gpiobackward);
        if(g_stepper)
        {
            g_stepper->setParameters(g_config->m_stepper.speed, g_config->m_stepper.accel, g_config->m_stepper.jerk);
            g_stepper->setLimits(g_config->m_stepper.minLimit, g_config->m_stepper.maxLimit);
        }
    }
}
ZControl::~ZControl()
{
    if(g_hx711)
        delete g_hx711;
    g_hx711 = nullptr;
    if(g_stepper)
        delete g_stepper;
    g_stepper = nullptr;
    if(g_config)
        delete g_config;
    g_config = nullptr;
}

void ZControl::setupGpio()
{
}
