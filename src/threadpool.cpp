#include "threadpool.h"
#include <unistd.h>
#include <iostream>

ThreadPool::ThreadPool(int threadNumber) :
    threadCount(threadNumber),
    counter(0)
{
    if(threadNumber == 0)
        threadCount = sysconf(_SC_NPROCESSORS_ONLN);

    for(size_t i = 0; i<threadCount; i++)
    {
        Client *t = new Client();
        t->start();
        threads.push_back(t);
    }

    std::cout<<"Create pool of "<<threadCount<<" threads"<<std::endl;
}

ThreadPool::~ThreadPool()
{
    for(size_t i = 0; i<threadCount; i++)
    {
        Client *cl = threads.at(i);
        cl->stop();
        delete cl;
    }
    threads.clear();

}

Client *ThreadPool::getNextThread()
{
    Client *cl = threads.at(counter % threadCount);
    ++counter;
    return cl;
}

