#include "stepper.h"

Stepper::Stepper(int gpiodir, int gpiopulse, int gpioenable, int gpiofault):
    RtThread(20)
{
    pinMode(gpiodir, INPUT);
    pinMode(gpiodir, OUTPUT);
    pinMode(gpiopulse, INPUT);
    pinMode(gpiopulse, OUTPUT);
    pinMode(gpioenable, INPUT);
    pinMode(gpioenable, OUTPUT);
    pinMode(gpiofault, INPUT);

    m_gpiodir = gpiodir;            // output direction signal
    m_gpiopulse = gpiopulse;        // output pulse signal
    m_gpioenable = gpioenable;      // output enable signal
    m_gpiofault = gpiofault;        // input fault signal
    m_offset = 0.;                  // offset value to convert pulse in deg
    m_gain = 1.8 / 4. / 16.;        // gain value to convert pulse in deg
    m_currentPosition = 0.;         // current position [deg]
    m_targetPosition = 0.;          // target position [deg]
    m_speed = 180. / 10.;           // speed value [deg / sec]
    m_acel = m_speed / 5.;          // aceleration value [deg / sec2]
    m_yerk = m_acel  / 1.;          // yerk value [deg / sec3]
    m_refered = false;              // axis refered
}
