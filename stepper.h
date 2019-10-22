#ifndef STEPPER_H
#define STEPPER_H

#include <QDebug>
#include "rtthread.h"
#include <string>
#include <atomic>
#include <wiringPi.h>

class Stepper: public RtThread
{
public:
    Stepper(int gpiodir, int gpiopulse, int gpioenable, int gpiofault);
private:
    int m_gpiodir;            // output direction signal
    int m_gpiopulse;          // output pulse signal
    int m_gpioenable;         // output enable signal
    int m_gpiofault;          // input fault signal

    double m_offset;          // offset value to convert pulse in deg
    double m_gain;            // gain value to convert pulse in deg
    double m_currentPosition; // current position [deg]
    double m_targetPosition;  // target position [deg]
    double m_speed;           // speed value [deg / sec]
    double m_acel;            // aceleration value [deg / sec2]
    double m_yerk;            // yerk value [deg / sec3]
    bool   m_refered;         // axis refered
};

#endif // STEPPER_H
