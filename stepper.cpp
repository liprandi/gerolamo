#include "stepper.h"
#include <unistd.h>
#include "math.h"

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

    m_gain = 16 * 4 / 1.8;          // convert deg -> pulse. Redutor de 16, motor 1.8 deg for pulse, drive x4
    m_offset = 0.;                  // in pulse value
    m_gpiodir = gpiodir;            // output direction signal
    m_gpiopulse = gpiopulse;        // output pulse signal
    m_gpioenable = gpioenable;      // output enable signal
    m_gpiofault = gpiofault;        // input fault signal
    m_offset = 0.;                  // offset value to convert pulse in deg
    m_gain = 4. * 16. / 1.8;        // gain value to convert pulse in deg
    m_currentPosition = 0.;         // current position [pulse]
    m_targetPosition = 0.;          // target position [pulse]
    m_override = 100;               // speed 100%
    setParameters(30, 5, 0.2);
    setLimits(0, 270);
    m_refered = false;              // axis refered
    start();
}

Stepper::~Stepper()
{

}
// set parameter for dynamic movement
void Stepper::setParameters(double speed, double accel, double jerk)
{
    m_speed = speed * m_gain;
    m_accel = accel * m_gain;
    m_jerk = jerk * m_gain;

    if(m_jerk > 0 && m_accel > 0)
    {
        m_jerkTime = m_accel / m_jerk;
        m_jerkPulse[0] = unsigned(1./ 6. * m_jerk * m_jerkTime * m_jerkTime * m_jerkTime);
        m_jerkSpeed = 1./ 2. * m_jerk * m_jerkTime * m_jerkTime;
        m_jerkPulse[1] = unsigned((m_speed - m_jerkSpeed) * m_jerkTime + 1./ 6. * m_jerk * m_jerkTime * m_jerkTime * m_jerkTime);
    }
    else
    {
        m_jerkTime = 0;
        m_jerkSpeed = 0;
        m_jerkPulse[0] = 0;
        m_jerkPulse[1] = 0;
    }
    if(m_accel > 0)
    {
        m_accelTime = (m_speed - 2 * m_jerkSpeed) / m_accel;
        m_accelPulse = unsigned(m_jerkSpeed * m_accelTime + 1. /2. * m_accel * m_accelTime * m_accelTime);
    }
    else
    {
        m_accelTime = 0;
        m_accelPulse = 0;
    }

}
// read parameters setted
void Stepper::getParameters(double& speed, double& accel, double& jerk)
{
    speed = double(m_speed) / m_gain;
    accel = double(m_accel) / m_gain;
    jerk = double(m_jerk) / m_gain;
}

// set limit position
void Stepper::setLimits(double minPosition, double maxPosition)
{
    m_minPosition = minPosition * m_gain + m_offset;
    m_maxPosition = maxPosition * m_gain + m_offset;
}
bool Stepper::go(double target)
{
    return go(long(target * m_gain + m_offset));
}
bool Stepper::go(long position)
{
    bool ret = false;
    if(!m_running && position != m_currentPosition)
    {
        if(position >= m_minPosition && position <= m_maxPosition)
        {
            ret = calcPulse(position);
            if(ret)
                m_targetPosition = position;
        }
    }
    return ret;
}

// return time in nsec for next pulse
bool Stepper::calcPulse(long position)
{
    bool ret = false;

    m_path.clear();
    unsigned distance = unsigned(abs(position - m_currentPosition));

    double t[3] = {0.5, 0.5, 0.5};
    double v[3] = {1/t[0], 1/t[1], 1/t[2]};
    double a[2] = {0., 0.};
    double j = 0.;
    for(unsigned pulse = 0; pulse < ((distance + 1) / 2); pulse++)
    {
        if(pulse < (distance / 4))
        {
            if(v[0] < m_speed)
            {
                j = m_jerk;
                a[1] = a[0];
                a[0] = m_jerk * t[0] + a[1];
                if(a[0] > m_accel)
                {
                    a[0] = m_accel;
                    j = 0;
                }
                v[2] = v[1];
                v[1] = v[0];
                v[0] = a[0] * t[0] + v[1];
                if(v[0] > m_speed)
                {
                    v[0] = m_speed;
                    a[0] = 0;
                    j = 0;
                }
                t[2] = t[1];
                t[1] = t[0];
                t[0] = 1 / v[0];
            }
            else
            {
                t[2] = t[1];
                t[1] = t[0];
                t[0] = 1 / m_speed;
                v[2] = v[1];
                v[1] = v[0];
                v[0] = m_speed;
                a[1] = a[0];
                a[0] = 0.;
                j = 0;
            }
        }
        else
        {
            if(v[0] < m_speed)
            {
                j = -m_jerk;
                a[1] = a[0];
                a[0] = j * t[0] + a[1];
                if(a[0] < 0)
                {
                    a[0] = 0;
                    j = 0;
                }
                v[2] = v[1];
                v[1] = v[0];
                v[0] = a[0] * t[0] + v[1];
                if(v[0] > m_speed)
                {
                    v[0] = m_speed;
                    a[0] = 0;
                    j = 0;
                }
                else if (v[0] < 2)
                {
                    v[0] = 2;
                    a[0] = 0;
                    j = 0;
                }

                t[2] = t[1];
                t[1] = t[0];
                t[0] = 1 / v[0];
            }
            else
            {
                t[2] = t[1];
                t[1] = t[0];
                t[0] = 1 / m_speed;
                v[2] = v[1];
                v[1] = v[0];
                v[0] = m_speed;
                a[1] = a[0];
                a[0] = 0.;
                j = 0;
            }
        }
        m_path[pulse] = int64_t(t[0] * 1e9);
        if(pulse < (distance - pulse - 1))
        {
            m_path[distance - pulse - 1] = int64_t(t[0] * 1e9);
        }
    }
    ret = (m_path.size() == distance);
    return ret;
}
// stepper task
void Stepper::run()
{
    struct timespec ts;

    // thread hx711 loop
    while(!m_quit)
    {
        if(digitalRead(m_gpiofault))
        {
            usleep(500000); //500 msec
            continue;
        }
        if(m_currentPosition != m_targetPosition)
        {
            if(!m_running)
            {
                m_direction = m_targetPosition > m_currentPosition;
                m_currentAccel = 0;
                m_currentJerk = 0;
                m_indexPath = 0;
                digitalWrite(m_gpioenable, 1);
                digitalWrite(m_gpiodir, m_direction);
                usleep(100000);
                clock_gettime(CLOCK_MONOTONIC, &ts);
                m_running = true;
            }
            if(m_running)
            {
                m_pulse = !m_pulse;
                digitalWrite(m_gpiopulse, m_pulse);
                if(m_indexPath < m_path.size())
                {
                    auto t = m_path[m_indexPath++];
                 // m_currentSpeed = t.speed;
                 // m_currentAccel = t.accel;
                 // m_currentJerk = t.jerk;
                    if(m_direction)
                        m_currentPosition++;
                    else
                        m_currentPosition--;
                    t *= 100L / m_override;
                    sleep_until(&ts, t);
                }
                else
                {
                    m_running = false;
                    usleep(500000); //500 msec
                }
            }
            else
            {
                usleep(500000); //500 msec
            }
        }
        else
        {
            m_running = false;
            usleep(500000); //500 msec
            digitalWrite(m_gpioenable, 0);
        }
    }
}
