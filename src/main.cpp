#include "sum.hpp"
#include "gx_log.h"
#include <utils/Mutex.h>
#include <utils/Condition.h>
#include <utils/Thread.h>
#include <utils/Errors.h>

extern int swap(int&, int&);
using namespace android;
class MyThread : public Thread
{
public:
    MyThread();
    virtual ~MyThread() {}
    int getDummy();
    bool threadLoop();
    virtual void onFirstRef();
private:
    int dummy;
};
void MyThread::onFirstRef()
{
    dummy = 30;
    LOGD("dummy = %d", dummy);
    LOGD("hello, onFirstRef this = %x, strongcount = %d", (unsigned long)this, this->getStrongCount());
}
MyThread::MyThread()
:Thread(false), dummy(100)
{
    LOGD("dummy = %d", dummy);
}
int MyThread::getDummy()
{
    return dummy;
}
bool MyThread::threadLoop()
{
    //dummy = 10;
    LOGD("dummy = %d", dummy);
    LOGD("hello, threadLoop this = %x, strongcount = %d", (unsigned long)this, this->getStrongCount());
    return false;
}

int main()
{
#if 1
    sp<MyThread> pt = new MyThread();
   // sp<MyThread> p2(pt);
    LOGD("main thread start = %d", pt->getDummy());
    if(pt->run("Hello-thread"))
    {
        LOGD("fail to start thread = %d", pt->getDummy());
    }
    LOGD("success to start thread = %d", pt->getDummy());
    pt->join();
    LOGD("main thread exit1 = %d, strongcount = %d", pt->getDummy(), pt->getStrongCount());
   // LOGD("main thread exit2 = %d", p2->getDummy());
    //delete pt;
#else
    MyThread th;
    th.run("Hello-thread");
    th.join();
    
#endif
    return 0; 
}
