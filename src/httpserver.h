#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <ev++.h>
#include "client.h"
#include "threadpool.h"

class Server
{
public:
    Server(int port);
    ~Server();
    void start();
protected:
   void ioAccept(ev::io &watcher, int revents);
   void signalCallback(ev::sig &signal, int revents);

   ThreadPool *threadPool = nullptr;


    ev::io io;
    ev::default_loop loop;
    ev::sig sio;
    int port;
    int s;


};

#endif // HTTPSERVER_H
