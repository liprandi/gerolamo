#ifndef RTTHREAD_H
#define RTTHREAD_H

#include <string>
#include <pthread.h>

class RtThread
{
public:
    RtThread(int priority);
    virtual ~RtThread();
    void start();
    void stop();
    virtual void run() = 0;
    static void *func(void *arg);
    static void sleep_until(struct timespec *ts, int64_t delay);
protected:
    [[ noreturn ]] static void panic(const std::string& msg);
private:
    pthread_t m_thread;
    static int m_numOfRtThreads;    // number of rt thread
protected:
    bool m_run;
    bool m_quit;
    int m_priority;
};

#endif // RTTHREAD_H
