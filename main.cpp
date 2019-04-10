#include "threadPool.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

class cMyTask : public cTask
{
public:
    cMyTask() = default;
    int run()
    {
        cout << (char*)pData << endl;
        int x = rand() % 4 + 1;
        sleep(x);
        return 0;
    }
    ~cMyTask() {}
};

int main()
{
    cMyTask taskObj;
    char szTmp[] = "hello!";
    taskObj.setData((void*)szTmp);
    cThreadPool threadPool(5);

    for(int i = 0; i < 10; i++)
        threadPool.addTask(&taskObj);

    while(1)
    {
        cout << "There are still" << threadPool.getTaskSize() << "tasks need to handle" << endl;
        if (threadPool.getTaskSize() == 0)
        {
            if (threadPool.stopAll() == -1)
            {
                cout << "Thread pool clear, exit." << endl;
                exit(0);
            }
        }
        sleep(2);
        cout << "2 seconds later..." << endl;
    }
    
    return 0;
}