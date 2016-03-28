#include <iostream>
#include "ev++.h"
#include "httpserver.h"

using namespace std;


int main()
{
    int port = 9989;

//    if(argc > 1)
//        port = atoi(argv[1]);


    Server server(port);
    server.start();





    return 0;
}

