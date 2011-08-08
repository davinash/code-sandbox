/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
 *  This file is part of code-sandbox project
 *  Copyright (C) 2011 Avinash Dongre ( dongre.avinash@gmail.com )
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/

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
