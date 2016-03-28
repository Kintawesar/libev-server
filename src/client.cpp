#include "client.h"
#include <fcntl.h>
#include <printf.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

//struct Session
//{
//    int clientSocket;
//};

Client::Client() : loop(ev::AUTO){}

void Client::callback(ev::io &watcher, int revents)
{
    if(EV_ERROR & revents)
    {
        printf("error in client callback");
        return;
    }

    if(revents & EV_READ)
    {
        readEv(watcher);

    }

    if(revents & EV_WRITE)
    {
        writeEv(watcher);

    }
}

void Client::readEv(ev::io &watcher)
{
    char buffer[1024];
    ssize_t readBytes = recv(watcher.fd, buffer, sizeof(buffer),0);
    if(readBytes < 0)
    {
        return;
    }
    else if(readBytes == 0)
    {
        removeClient(watcher.fd);
    }
    else
    {
//        std::cout<<buffer<<std::endl;
        io.set(ev::WRITE);
    }

}

std::string Client::makeHttpAnswer()
{
    std::string http;
    char body[] = "<html><body><center><h1>Hello World!</h1></center></body></html>";
    std::cout<< sizeof(body) <<std::endl;

    http.append("HTTP/1.1 200 OK\r\n");
    http.append("Date: Wed, 11 Feb 2009 11:20:59 GMT\r\n");
    http.append("Server: Apache\r\n");
    http.append("X-Powered-By: PHP/5.2.4-2ubuntu5wm1\r\n");
    http.append("Last-Modified: Wed, 11 Feb 2009 11:20:59 GMT\r\n");
    http.append("Content-Language: ru\r\n");
    http.append("Content-Type: text/html; charset=utf-8\r\n");
    http.append("Content-Length: 65\r\n");
    http.append("Connection: close\r\n");
    http.append("\r\n");
    http.append(body);

    return http;
}

void Client::writeEv(ev::io &watcher)
{
    std::string http = makeHttpAnswer();

    ssize_t bytesWritten = write(watcher.fd,http.c_str(),http.length());
    if(bytesWritten < 0)
    {
        std::cout<<"error write bytes"<<std::endl;
        return;
    }
    else if(bytesWritten == 0)
    {
        removeClient(watcher.fd);
    }
    else
    {
        removeClient(watcher.fd);
//        stop();
//        io.set(ev::READ);

    }

}
Client::~Client()
{
//    stop();
}

void Client::stopCallback()
{
    stopper.stop();
    loop.break_loop(ev::ALL);
}

void Client::run()
{

   io.set<Client, &Client::callback>(this);
   stopper.set(loop);
   stopper.set<Client, &Client::stopCallback>(this);
   stopper.start();


   loop.run(0);
   std::cout<<"Thread "<<thread.get_id() <<"start "<<std::endl;
}

void Client::addClient(int client_socket)
{

    fcntl(client_socket, F_SETFL, fcntl(client_socket, F_GETFL,0) | O_NONBLOCK);
    std::cout<<"got connection in thread "<<thread.get_id()<<std::endl;

    io.start(client_socket, ev::READ);
//    sockets.push_back(client_socket);
}

void Client::removeClient(int socket)
{
    shutdown(socket,SHUT_RDWR);
    close(socket);
    std::cout<<"close socket"<<std::endl;
}

void Client::stop()
{
//    for(auto it = sockets.cbegin(); it != sockets.cend(); ++it)
//    {
//        removeClient(*it);

//    }
//    sockets.clear();


    std::cout<<"stop thread "<<thread.get_id() <<std::endl;
    stopper.send();
    thread.join();
}

void Client::start()
{
    thread = std::thread(&Client::run, this);
}

