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
    m_gain = 1.8 / 4. / 16.;        // gain value to convert pulse in deg
    m_currentPosition = 0.;         // current position [pulse]
    m_targetPosition = 0.;          // target position [pulse]
    m_override = 100;               // speed 100%
    setParameters(100, 50, 25);
    setLimits(0, 270);
    m_refered = false;              // axis refered
}

Stepper::~Stepper()
{

}
// set parameter for dynamic movement
void Stepper::setParameters(double speed, double accel, double jerk)
{
    m_speed = speed * m_gain + m_offset;
    m_accel = accel * m_gain + m_offset;
    m_jerk = jerk * m_gain + m_offset;

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
// set limit position
void Stepper::setLimits(double minPosition, double maxPosition)
{
    m_minPosition = minPosition * m_gain + m_offset;
    m_maxPosition = maxPosition * m_gain + m_offset;
}
// return time in nsec for next pulse
bool Stepper::calcPulse()
{
    bool ret = false;

    unsigned distance = unsigned(abs(m_targetPosition - m_startPosition));
    double jerk[2] = {m_currentJerk, 0};
    double accel[2] = {m_currentAccel, 0};
    double speed[2] = {m_currentSpeed, 0};
    unsigned minSpace = 2 * m_accelPulse + 2 * m_jerkPulse[0] + 2 * m_jerkPulse[1];
    if(distance >= minSpace)
    for(unsigned pulse = 0; pulse < distance; pulse++)
    {
        unsigned neg = distance - pulse;
        if(speed[0] < m_speed)
        {
            if(pulse < m_jerkPulse)
            {
                jerk[0] = m_jerk;
                accel[0] += jerk[0];
            }
            else
            {
                accel[0] = m_accel;
                jerk[0] = 0;
            }
            speed[0] += accel[0];
            if(speed[0] > m_speed)
                speed[0] = m_speed;
        }
    }
    if(m_targetPosition != m_currentPosition)
    {
        long diff = m_targetPosition - m_currentPosition;
        if(diff > 0)
        {
            if(abs(m_currentSpeed - m_speed) < 0.1)
            {
                if(diff > (2 * m_jerkPulse + m_accelPulse))
                {
                    // normal course at maximum speed
                    m_currentSpeed = m_speed;
                    m_currentAccel = 0;
                    m_currentJerk = 0;
                    ret = long(1e9/m_speed);
                }
                else
                {
                    // slew down
                    if(diff > (m_jerkPulse + m_accelPulse))
                    {
                        // first slew down jerk
                        m_currentJerk = -m_jerk;
                        double deltaTime = pow(6. / m_jerk, 1./3.);
                        m_currentAccel += m_currentJerk * deltaTime;
                        m_currentSpeed += 1./2. * m_currentJerk * deltaTime * deltaTime;
                        ret = long((deltaTime + sqrt(2. / m_currentAccel) + 1 / m_currentSpeed) * 1e9);
                    }
                    else
                    {
                        if(diff > m_jerkPulse)
                        {
                            // ramp down
                            m_currentJerk = 0;
                            m_currentAccel = -m_accel;
                            double deltaTime = sqrt(2. / m_accel);
                            m_currentSpeed += m_currentAccel * deltaTime;
                            ret = long(deltaTime * 1e9);
                        }
                        else
                        {
                            // second slew down jerk
                            m_currentJerk = -m_jerk;
                            double deltaTime = pow(6. / m_jerk, 1./3.);
                            m_currentAccel += m_currentJerk * deltaTime;
                            m_currentSpeed += 1./2. * m_currentJerk * deltaTime * deltaTime;
                            ret = long(deltaTime * 1e9);
                        }
                    }
                }

            }
        }
    }
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
                m_startPosition = m_currentPosition;
                m_direction = m_targetPosition > m_currentPosition;
                m_currentAccel = 0;
                m_currentJerk = 0;
                m_indexPath = 0;
                m_running = calcPulse();
                if(m_running)
                {
                    digitalWrite(m_gpioenable, 1);
                    clock_gettime(CLOCK_MONOTONIC, &ts);
                }
            }
            if(m_running)
            {
                digitalWrite(m_gpiodir, m_direction);
                m_pulse = !m_pulse;
                digitalWrite(m_gpiopulse, m_pulse);
                if(m_indexPath < m_path.size())
                {
                    auto t = m_path[m_indexPath++];
                    m_currentSpeed = t.speed;
                    m_currentAccel = t.accel;
                    m_currentJerk = t.jerk;
                    if(m_direction)
                        m_currentPosition++;
                    else
                        m_currentPosition--;
                    t.delay *= 100L / m_override;
                    sleep_until(&ts, t.delay);
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
            usleep(500000); //500 msec
            digitalWrite(m_gpioenable, 0);
        }
    }
}
