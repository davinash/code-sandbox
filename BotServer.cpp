#include "BotServerSocket.h"
#include "SocketException.h"
#include "HttpRequestHandler.h"
#include <string>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdio.h>

#define SERVER_PORT 1700
void* request_handler(void *param) {
    /*std::cout << "======================================" << std::endl;
    std::cout << "Handling the request " << std::endl;*/
    int client_local = *((int *)param);
    HttpRequestHandler handle;
    handle.perform_request(client_local);
    close(client_local);
}

int main ( int argc, char** argv ) {
    //std::cout << "running....\n";
    struct sockaddr_in cliAddr; 
    int incoming;
    int addr_len;
    pthread_t thread;
    try {
        // Create the socket
        BotServerSocket server (SERVER_PORT); 
        while ( 1 ) {
            /*std::cout << argv[0] << " waiting for data on port TCP " 
                      << SERVER_PORT << std::endl;*/

            incoming = server.accept();
            if ( incoming < 0 ) {
                perror ( "Cannot accept connection" );
                break;
            }
            pthread_create( &thread,
                            NULL,
                            request_handler,
                            (void*)&incoming);
        }
    }
    catch ( SocketException& e ) {
        std::cout << "Exception was caught:" 
                  << e.description() << "\nExiting.\n";
    }
    return 0;
}
