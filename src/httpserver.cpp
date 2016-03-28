#include "httpserver.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <event.h>
#include <iostream>


#define SOCKET_LISTEN 5

Server::Server(int port) : port(port)
{

}

void Server::start()
{
    s = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if(bind(s, (sockaddr *)&addr,sizeof(addr) ) != 0)
    {
        std::cout<<"error bind socket, maybe port is busy"<<std::endl;
    }

    //non-block
    int flags = fcntl(s,F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(s,F_SETFL,flags);

    listen(s,SOCKET_LISTEN);

    io.set<Server, &Server::ioAccept>(this);
    io.start(s, ev::READ);

    sio.set<Server, &Server::signalCallback>(this);
    sio.start(SIGINT);

    std::cout<<"server started"<<std::endl;

    threadPool = new ThreadPool();

    loop.run(0);
}

void Server::ioAccept(ev::io &watcher, int revents)
{
    if(EV_ERROR & revents)
    {
        return;
    }

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    int client_socket = accept(watcher.fd, (sockaddr*)&client_addr, &len);

    if(client_socket < 0)
    {
        std::cout<<"client socket < 0"<<std::endl;
        return;
    }

    Client *client = threadPool->getNextThread();
    client->addClient(client_socket);

}

void Server::signalCallback(ev::sig &signal, int revents)
{
    shutdown(s, SHUT_RDWR);
    close(s);

    signal.loop.break_loop(ev::ALL);
    signal.stop();

    delete threadPool;
}

Server::~Server()
{
    shutdown(s, SHUT_RDWR);
    close(s);

    sio.stop();
    ev_default_destroy();

    delete threadPool;

}

