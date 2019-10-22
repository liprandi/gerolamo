#ifndef HX711_H
#define HX711_H

#include <QDebug>
#include "rtthread.h"
#include <string>
#include <atomic>
#include <wiringPi.h>


class Hx711: public RtThread
{
public:
    enum
    {
        k_bufferSize = 100,
        k_fifoSize   = 20,
        k_maxChn     = 3
    };

    Hx711(int gpioclk, int gpiodata1, int gpiodata2, int gpiodata3);
    virtual ~Hx711();
    void run();
    inline double value(int chn){return m_value[chn];}
    inline double filtered(int chn){return m_filteredValue[chn];}
    inline double average(int chn){return m_avgValue[chn];}
    void readChannels();
    bool checkValid();
    void setValues();
    void resetChip();
private:
    [[ noreturn ]] void panic(const std::string& msg);
private:
    int m_numOfChannels;                              // num of chnnels read
    int m_gpioClk;                                    // output pin used for clock
    int m_gpioData[k_maxChn];                         // inputs pins used for read
    int m_makingValue[k_maxChn];                      // reading values
    int m_fifoValues[k_maxChn][k_fifoSize];           // fifo list of last values read
    double m_offset[k_maxChn];                        // conversion offset
    double m_gain[k_maxChn];                          // conversion gain value
    std::atomic<double> m_value[k_maxChn];            // last value
    std::atomic<double> m_filteredValue[k_maxChn];    // current value of last 5 read
    std::atomic<double> m_avgValue[k_maxChn];         // current value for last fifo values
public:
    struct timespec m_ts[k_bufferSize][50];           // fifo buffer
    unsigned m_ts_index;                              // current index circluar buffer
    unsigned m_ts_count;                              // counter read values
    unsigned m_ts_reset;                              // counter error (needed reset)
public:
    decltype(m_ts)& getStatistic(unsigned& count, unsigned& errors, unsigned& index)
    {
        count = m_ts_count;
        index = m_ts_index;
        errors = m_ts_reset;
        return m_ts;
    }
};

#endif // HX711_H
