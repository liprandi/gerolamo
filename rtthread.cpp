#include "rtthread.h"
#include <QDebug>
#include <sys/mman.h>

int RtThread::m_numOfRtThreads = 0;

RtThread::RtThread(int priority):
    m_priority(priority)
{
    m_run = false;
    m_quit = false;
}

RtThread::~RtThread()
{
    stop();
}

void RtThread::start()
{
    if(m_numOfRtThreads == 0)
    {
        if(mlockall(MCL_FUTURE|MCL_CURRENT))
            panic("WARNING: Could not lock memory with mlockall()");
    }
    ++m_numOfRtThreads;
    // Create our main threads
    ::pthread_create(&m_thread, nullptr, func, reinterpret_cast<void *>(this));
}
void RtThread::stop()
{
    if(m_run)
    {
        m_quit = true;
        void* dummy;
        ::pthread_join(m_thread, &dummy);
        if(--m_numOfRtThreads == 0)
            munlockall();
    }
    m_quit = false;
}

// General purpose error message
void RtThread::panic(const std::string& msg)
{
    qDebug() << msg.data();
    exit(1);
}
void * RtThread::func(void *arg)
{
    RtThread* super = static_cast<RtThread*>(arg);
    // Ensure that we have real time priority set
    struct sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = super->m_priority;
    if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp))
        panic("WARNING: Failed to set stepper thread to realtime priority");

    super->m_run = true;
    super->run();
    super->m_run = false;
    return nullptr;
}
// Adds "delay" nanoseconds to timespecs and sleeps until that time
void RtThread::sleep_until(struct timespec *ts, int64_t delay)
{
    int64_t sec = delay / (1000L*1000L*1000L);
    int64_t nsec = delay %  (1000L*1000L*1000L);
    ts->tv_nsec += long(nsec);
    if(ts->tv_nsec > (1000*1000*1000))
    {
        ts->tv_nsec -= (1000*1000*1000);
        sec++;
    }
    ts->tv_sec += long(sec);
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, ts,  nullptr);
}


