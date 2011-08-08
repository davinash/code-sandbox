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
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
 *  USE OR OTHER DEALINGS IN  THE SOFTWARE.
 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/

#include "SandboxContext.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 1700

int main ( int argc, char** argv ) {

    SandboxContext context;
    int rc = context.init_context(argc, argv);
    if ( rc != 0 ) {
        std::cout << "Invalid Configuration " << std::endl;
        exit ( EXIT_FAILURE);
    }
    rc = context.m_server_socket->init_server_socket(SERVER_PORT);
    if ( rc != 0 ) {
        std::cout << "Socket Initialization Failed" << std::endl;
        exit ( EXIT_FAILURE);
    }
    rc = context.m_socket_listner->start_listening();
    return 0;
}
