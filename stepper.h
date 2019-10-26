#ifndef STEPPER_H
#define STEPPER_H

#include <QDebug>
#include "rtthread.h"
#include <string>
#include <atomic>
#include <wiringPi.h>


class Stepper: public RtThread
{
    struct pulseInfo
    {
        int64_t delay;      // delay to next step
        double speed;       // speed of this step
        double accel;       // accel of this step
        double jerk;        // jerk of this step
    };

public:
    Stepper(int gpiodir, int gpiopulse, int gpioenable, int gpiofault);
    virtual ~Stepper();
    void run();

    void setParameters(double speed, double accel, double jerk);
    void setLimits(double minPosition, double maxPosition);
    bool calcPulse(); // return true if was able to calculate path
private:
    int m_gpiodir;            // output direction signal
    int m_gpiopulse;          // output pulse signal
    int m_gpioenable;         // output enable signal
    int m_gpiofault;          // input fault signal
    // values in deg for general purpose
    double m_offset;          // offset value to convert pulse in deg
    double m_gain;            // gain value to convert pulse in deg
    // values in pulse for calculation
    long m_currentPosition;   // current position [pulse]
    double m_currentSpeed;    // current speed [pulse/sec]
    double m_currentAccel;    // current accel [pulse/sec2]
    double m_currentJerk;     // current accel [pulse/sec3]
    long m_startPosition;     // target position [pulse]
    long m_targetPosition;     // target position [pulse]
    double m_minPosition;     // limit min position [pulse]
    double m_maxPosition;     // limit max position [pulse]
    double m_speed;           // speed value [pulse / sec]
    double m_accel;           // acceleration value [pulse / sec2]
    double m_accelTime;       // acceleration time between jerk transitions
    unsigned m_accelPulse;         // acceleration pulses during between jerk trasitions
    double m_jerk;            // jerk value [pulse / sec3]
    double m_jerkSpeed;       // speed value reached after jerk transition
    double m_jerkTime;        // time necessary for jerk transition to reach accel value
    unsigned m_jerkPulse[2];       // position moved during jerk transition
    bool   m_refered;         // axis refered
    bool   m_direction;       // current direction
    bool   m_pulse;           // current direction
    bool   m_running;         // the motor is running
    unsigned m_indexPath;       // index of current path running
    int64_t m_override;       // [10-100] override
    std::vector<pulseInfo> m_path;    // pulse time list
};

#endif // STEPPER_H
