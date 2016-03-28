#ifndef CLIENT_H
#define CLIENT_H
#include "ev++.h"
#include <thread>
#include <vector>

class Client
{
public:
    typedef std::thread thread_t;
    Client();
    ~Client();
    void run();
    void addClient(int client_socket);

    void start();
    void stop();
private:
    void callback(ev::io &watcher, int revents);
    void readEv(ev::io &watcher);
    void writeEv(ev::io &watcher);

    std::vector<int> sockets;
    ev::dynamic_loop loop;

    thread_t thread;
    int socket;
    ev::io io;
    ev::async stopper;
    std::string makeHttpAnswer();
    void removeClient(int socket);
    void stopCallback();
};

#endif // CLIENT_H
