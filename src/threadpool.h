#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "client.h"
#include <vector>

class ThreadPool
{
public:
    ThreadPool(int threadNumber = 0);
    ~ThreadPool();
    Client *getNextThread();


private:

    std::vector<Client*>threads;
    size_t counter;
    size_t threadCount;
};

#endif // THREADPOOL_H
