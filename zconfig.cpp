#include "zconfig.h"
#include <QSettings>

ZConfig::ZConfig()
{
    QSettings set("CamDoBrasil", "Gerolamo");
    // read setting of Hx711
    m_hx711.gpioclk = set.value("Hx711/GPIOclk", 4).toInt();
    m_hx711.gpiodata1 = set.value("Hx711/GPIOdata1", 14).toInt();
    m_hx711.gpiodata2 = set.value("Hx711/GPIOdata2", 15).toInt();
    m_hx711.gpiodata3 = set.value("Hx711/GPIOdata3", 18).toInt();
    m_hx711.offset1 = set.value("Hx711/offset1", 0.).toDouble();
    m_hx711.offset2 = set.value("Hx711/offset2", 0.).toDouble();
    m_hx711.offset3 = set.value("Hx711/offset3", 0.).toDouble();
    m_hx711.gain1 = set.value("Hx711/gain1", 0.).toDouble();
    m_hx711.gain2 = set.value("Hx711/gain2", 0.).toDouble();
    m_hx711.gain3 = set.value("Hx711/gain3", 0.).toDouble();
    // read setting of Stepper
    m_stepper.minLimit = set.value("Stepper/minLimit", 0.).toDouble();
    m_stepper.maxLimit = set.value("Stepper/maxLimit", 270.).toDouble();
    m_stepper.speed = set.value("Stepper/speed", 100.).toDouble();
    m_stepper.accel = set.value("Stepper/accel", 10.).toDouble();
    m_stepper.jerk = set.value("Stepper/jerk", 1.).toDouble();
    m_stepper.override = set.value("Stepper/override", 100.).toDouble();
    m_stepper.refForward = set.value("Stepper/refFordward", 260.).toDouble();
    m_stepper.refBackward = set.value("Stepper/refFordward", 10.).toDouble();
    m_stepper.offset = set.value("Stepper/offset", 0.).toDouble();
    m_stepper.gain = set.value("Stepper/gain", 16. * 4. / 1.8).toDouble();
    m_stepper.gpiodir = set.value("Hx711/GPIOdir", 27).toInt();
    m_stepper.gpiopulse = set.value("Hx711/GPIOpulse", 22).toInt();
    m_stepper.gpioenable = set.value("Hx711/GPIOenable", 17).toInt();
    m_stepper.gpiofault = set.value("Hx711/GPIOfault", 23).toInt();
    m_stepper.gpioforward = set.value("Hx711/GPIOforward", 24).toInt();
    m_stepper.gpiobackward = set.value("Hx711/GPIObackward", 10).toInt();
}
ZConfig::~ZConfig()
{
    QSettings set("CamDoBrasil", "Gerolamo");
    // write setting of Hx711
    set.setValue("Hx711/GPIOclk", m_hx711.gpioclk);
    set.setValue("Hx711/GPIOdata1", m_hx711.gpiodata1);
    set.setValue("Hx711/GPIOdata2", m_hx711.gpiodata2);
    set.setValue("Hx711/GPIOdata3", m_hx711.gpiodata3);
    set.setValue("Hx711/offset1", m_hx711.offset1);
    set.setValue("Hx711/offset2", m_hx711.offset2);
    set.setValue("Hx711/offset3", m_hx711.offset3);
    set.setValue("Hx711/gain1", m_hx711.gain1);
    set.setValue("Hx711/gain2", m_hx711.gain2);
    set.setValue("Hx711/gain3", m_hx711.gain3);
    // write setting of Stepper
    set.setValue("Stepper/minLimit", m_stepper.minLimit);
    set.setValue("Stepper/maxLimit", m_stepper.maxLimit);
    set.setValue("Stepper/speed", m_stepper.speed);
    set.setValue("Stepper/accel", m_stepper.accel);
    set.setValue("Stepper/jerk", m_stepper.jerk);
    set.setValue("Stepper/override", m_stepper.override);
    set.setValue("Stepper/refFordward", m_stepper.refForward);
    set.setValue("Stepper/refFordward", m_stepper.refBackward);
    set.setValue("Stepper/offset", m_stepper.offset);
    set.setValue("Stepper/gain", m_stepper.gain);
    set.setValue("Hx711/GPIOdir", m_stepper.gpiodir);
    set.setValue("Hx711/GPIOpulse", m_stepper.gpiopulse);
    set.setValue("Hx711/GPIOenable", m_stepper.gpioenable);
    set.setValue("Hx711/GPIOfault", m_stepper.gpiofault);
    set.setValue("Hx711/GPIOforward", m_stepper.gpioforward);
    set.setValue("Hx711/GPIObackward", m_stepper.gpiobackward);
}
