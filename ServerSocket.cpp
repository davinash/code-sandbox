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
#include "ServerSocket.h"
#include "SocketException.h"

ServerSocket::ServerSocket() {
}

int ServerSocket::init_server_socket ( int port ) {
    if ( ! Socket::create() ) {
        return -1;
    }
    if ( ! Socket::bind ( port ) ) {
        return -2;
    }
    if ( ! Socket::listen() ) {
        return -3;
    }
}

ServerSocket::~ServerSocket() { }
const ServerSocket& ServerSocket::
                operator << ( const std::string& s ) const { 
    if ( ! Socket::send ( s ) ) {
        throw SocketException ( "Could not write to socket." );
    }
    return *this;
}

const ServerSocket& ServerSocket::operator >> ( std::string& s ) const {
    if ( ! Socket::recv ( s ) ) {
        throw SocketException ( "Could not read from socket." );
    }
    return *this;
}

int ServerSocket::accept () {
    return Socket::accept ();
}

