#include <sys/mman.h>
#include <unistd.h>
#include "hx711.h"

Hx711::Hx711(int gpioclk, int gpiodata1, int gpiodata2, int gpiodata3):
    RtThread(30)
{
    m_ts_index = 0;
    m_ts_count = 0;
    m_ts_reset = 0;
    m_numOfChannels = 0;

    pinMode(gpioclk, INPUT);
    pinMode(gpioclk, OUTPUT);
    m_gpioClk = gpioclk;
    m_gpioData[0] = m_gpioData[1] = m_gpioData[2] = -1;
    if(gpiodata1 >= 2 && gpiodata1 <= 26)
    {
        m_numOfChannels++;
        pinMode(gpiodata1, INPUT);
        m_gpioData[0] = gpiodata1;
        if(gpiodata2 >= 2 && gpiodata2 <= 26 && gpiodata1 != gpiodata2)
        {
            m_numOfChannels++;
            pinMode(gpiodata2, INPUT) ;
            m_gpioData[1] = gpiodata2;
            if(gpiodata3 >= 2 && gpiodata3 <= 26 && gpiodata1 != gpiodata3 && gpiodata2 != gpiodata3)
            {
                m_numOfChannels++;
                pinMode(gpiodata3, INPUT) ;
                m_gpioData[2] = gpiodata3;
            }
        }
    }
    for(int chn = 0; chn < m_numOfChannels; chn++)
    {
        m_offset[chn] = 0.;
        m_gain[chn] = 1.;
    }
    if(m_numOfChannels > 0)
    {
        start();
    }
}

Hx711::~Hx711()
{

}

void Hx711::run()
{
    bool start;
    struct timespec ts;
    int chn;

    // thread hx711 loop
    while(!m_quit)
    {
        digitalWrite(m_gpioClk, 0);     // start with clock to zero
        start = false;
        if(m_numOfChannels > 0 && !digitalRead(m_gpioData[0]))
            if(m_numOfChannels < 2 || !digitalRead(m_gpioData[1]))
                if(m_numOfChannels < 3 || !digitalRead(m_gpioData[2]))
                    start = true;

        // Initialize timespec from the monotonically increasing clock source
        clock_gettime(CLOCK_MONOTONIC, &ts);
        if(start)
        {
            if(++m_ts_index >= k_bufferSize)
                m_ts_index = 0;
            m_ts_count++;
            for(chn = 0; chn < m_numOfChannels; chn++)
                m_makingValue[chn] = 0;
            readChannels();
            if(checkValid())
            {
                setValues();
            }
            else
            {
                resetChip();
            }
        }
        sleep_until(&ts, (100L * 1000L * 1000L));    // 100 msec
    }
}
// read the values of chips
void Hx711::readChannels()
{
    for(int i = 0; i < 25; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &m_ts[m_ts_index][i * 2]);
        digitalWrite(m_gpioClk, 1);     // clock to one
        for(volatile int t = 0; t < 2; t++){} // loose time
        digitalWrite(m_gpioClk, 0);     // clock to one
        clock_gettime(CLOCK_MONOTONIC, &m_ts[m_ts_index][i * 2 + 1]);
        if(i < (25-1))
        {
            for(int chn = 0; chn < m_numOfChannels; chn++)
            {
                m_makingValue[chn] <<= 1;
                if(digitalRead(m_gpioData[chn]))
                    m_makingValue[chn]++;
            }
        }
    }
}
// check if during the read we have some signals too slows
bool Hx711::checkValid()
{
    int64_t diff;
    auto& data = m_ts[m_ts_index];
    for(int c = 0; c < 25; c++)
    {
        // signal high
        diff = int64_t(data[c * 2 + 1].tv_sec) - int64_t(data[c * 2].tv_sec);
        diff *= 1000L * 1000L * 1000L;
        diff += int64_t(data[c * 2 + 1].tv_nsec) - int64_t(data[c * 2].tv_nsec);
        if(diff > 50L * 1000L)  // greater as 50 usec
            return false;
        if(diff < 200L)
            return false;
        // signal low
        if(c < (25 - 1))
        {
            diff = int64_t(data[c * 2 + 2].tv_sec) - int64_t(data[c * 2 + 1].tv_sec);
            diff *= 1000L * 1000L * 1000L;
            diff += int64_t(data[c * 2 + 2].tv_nsec) - int64_t(data[c * 2 + 1].tv_nsec);
            if(diff < 200L)
                return false;
        }
    }
    return true;
}
// set values for the main program
void Hx711::setValues()
{
    double avg5[k_maxChn] = {0., 0., 0.};
    double avg[k_maxChn] = {0., 0., 0.};
    for(int chn = 0; chn < m_numOfChannels; chn++)
    {
        m_value[chn] = m_offset[chn] + m_gain[chn] * double(m_makingValue[chn]);
        for(int idx = (k_fifoSize - 1); idx > 0; idx--)
        {
            m_fifoValues[chn][idx] = m_fifoValues[chn][idx - 1];
            avg[chn] += m_fifoValues[chn][idx];
            if(idx < 5)
                avg5[chn] += m_fifoValues[chn][idx];
        }
        m_fifoValues[chn][0] = m_makingValue[chn];
        m_avgValue[chn] = m_offset[chn] + m_gain[chn] * avg[chn] / double(k_fifoSize);
        m_filteredValue[chn] = m_offset[chn] + m_gain[chn] * avg5[chn] / 5.;
    }
}
// call to reaset chip hx711
void Hx711::resetChip()
{
    m_ts_reset++;
    usleep(60);
    digitalWrite(m_gpioClk, 1);     // clock to one
    usleep(60);
    digitalWrite(m_gpioClk, 0);     // clock to one
    usleep(60);
}
