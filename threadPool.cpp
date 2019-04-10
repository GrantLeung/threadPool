#include "threadPool.h"
#include <iostream>

void cTask::setData(void* data)
{
    pData = data;
}

vector<cTask* > cThreadPool::vecTaskList;
bool cThreadPool::isShutdown = false;
pthread_mutex_t cThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;

cThreadPool::cThreadPool(int threadNum)
{
    this->threadNum = threadNum;
    string str = "";
    cout << "I will create " << threadNum << " threads. " << endl;
    create();
}

void* cThreadPool::threadFunc(void* threadData)
{
    pthread_t tid = pthread_self();
    while(1)
    {
        pthread_mutex_lock(&m_pthreadMutex);

        while(vecTaskList.size() == 0 && !isShutdown)
            /* if list is null,then new task enter the task list */
            pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);
        if (isShutdown)
        {
            pthread_mutex_unlock(&m_pthreadMutex);
            cout << "[tid:" << pthread_self() << "]" << " exit" << endl;
            pthread_exit(nullptr);
        }
        cout << "[tid:" << tid << "]" << " run:";

        cTask* task;
        auto iter = vecTaskList.begin();
        if(iter != vecTaskList.end())
        {
            task = *iter;
            vecTaskList.erase(iter);
        }

        pthread_mutex_unlock(&m_pthreadMutex);
        
        task->run();
        cout << "[tid:" << tid << "]" << " idel" << endl;
    }
    return (void*)0;
}

/* add task into tasklist and send pthread_cond_signal */
int cThreadPool::addTask(cTask* task)
{
    pthread_mutex_lock(&m_pthreadMutex);
    vecTaskList.push_back(task);
    pthread_mutex_unlock(&m_pthreadMutex);
    pthread_cond_signal(&m_pthreadCond);
}


/* create thread */
int cThreadPool::create()
{
    pth_id = new pthread_t[threadNum];
    for(int i = 0; i < threadNum; i++)
        pthread_create(&pth_id[i], nullptr, threadFunc, nullptr);
    return 0;
}

/* stop all threads*/
int cThreadPool::stopAll()
{
    if (isShutdown)
        return -1;
    cout << "Now I will end all threads!" << endl;

    isShutdown = true;
    pthread_cond_broadcast(&m_pthreadCond);

    for (int i = 0; i < threadNum; i++)
        pthread_join(pth_id[i], nullptr);
    delete[] pth_id;
    pth_id = nullptr;

    pthread_mutex_destroy(&m_pthreadMutex);
    pthread_cond_destroy(&m_pthreadCond);

    return 0;
}

int cThreadPool::getTaskSize()
{
    return vecTaskList.size();
}
