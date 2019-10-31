#ifndef ZCONFIG_H
#define ZCONFIG_H


class ZConfig
{
public:
    struct Hx711
    {
        int gpioclk;        // output signal clock common
        int gpiodata1;      // input signal first Hx711
        int gpiodata2;      // input signal second Hx711
        int gpiodata3;      // input signal third Hx711
        double offset1;     // offset value for first Hx711
        double offset2;     // offset value for second Hx711
        double offset3;     // offset value for third Hx711
        double gain1;       // gain value for first Hx711
        double gain2;       // gain value for second Hx711
        double gain3;       // gain value for third Hx711
    };

    struct Stepper
    {
        double minLimit;      // min position limit
        double maxLimit;      // max position limit
        double speed;         // max speed value
        double accel;         // max accel value
        double jerk;          // max jerk value
        double override;      // ovverride percent value
        double refForward;    // reference value when reach forward signal
        double refBackward;   // reference value when reach backward signal
        double offset;        // offset value for stepper motor
        double gain;          // gain value for stepper motor
        int gpiodir;          // output direction signal
        int gpiopulse;        // output pulse signal
        int gpioenable;       // output enable signal
        int gpiofault;        // input fault signal
        int gpioforward;      // input signal for forward position
        int gpiobackward;     // input signal for backward position
    };
public:
    ZConfig();
    ~ZConfig();
public:
    Stepper m_stepper;           // config data for stepper
    Hx711 m_hx711;             // config data for hx711 modules
};

#endif // ZCONFIG_H
