#ifndef __THREADPOOL_H
#define __THREADPOOL_H

#include <iostream>
#include <vector>
#include <cstring>
#include <pthread.h>
using namespace std;

/* task class */
class cTask 
{
protected:
    string strTaskName;
    void* pData;

public:
    cTask() = default;
    cTask(string &taskName): strTaskName(taskName), pData(nullptr) {}
    virtual int run() = 0;
    void setData(void* data);

    virtual ~cTask() {}
};

/* thread class */
class cThreadPool
{
private:
    static vector<cTask* > vecTaskList;
    static bool isShutdown;
    int threadNum;
    pthread_t* pth_id;

    static pthread_mutex_t m_pthreadMutex;
    static pthread_cond_t m_pthreadCond;

protected:
    static void* threadFunc(void* threadData);
    static int moveToIdel(pthread_t tid);
    static int moveToBusy(pthread_t tid);
    int create();

public:
    cThreadPool(int threadNum);
    int addTask(cTask* task);
    int stopAll();
    int getTaskSize();
};

#endif 