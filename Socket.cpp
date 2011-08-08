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
#include "Socket.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

Socket::Socket() : m_sock ( -1 ) {
    memset ( &m_addr, 0, sizeof ( m_addr ) ); 
}

Socket::~Socket() {
    if ( is_valid() )
        ::close ( m_sock );
}

bool Socket::create() {
    m_sock = socket ( PF_INET, SOCK_STREAM, 0 );
    if ( ! is_valid() )
        return false;

    int on = 1;
    if ( setsockopt ( m_sock, SOL_SOCKET, 
                      SO_REUSEADDR, (const char*)&on, 
                      sizeof(int)) == -1 )
        return false;
    return true;
}
bool Socket::bind ( const int port ) {
    if ( ! is_valid() ) {
        return false;
    }
    bzero((void*)&m_addr,sizeof(m_addr));
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_addr.sin_port        = htons ( port );
    m_addr.sin_family      = AF_INET;
    int bind_return = ::bind ( m_sock, 
                               (struct sockaddr *)&m_addr, 
                               sizeof(m_addr));
    if ( bind_return == -1 ) {
        return false;
    }
    return true;
}

bool Socket::listen() const {
    if ( ! is_valid() ) {
        return false;
    }
    int listen_return = ::listen ( m_sock, MAXCONNECTIONS );
    if ( listen_return == -1 ) {
        return false;
    }
    return true;
}

int Socket::accept () const {
    int addr_length = sizeof ( m_addr );
    return(::accept ( m_sock, 
                      (sockaddr *)&m_addr, 
                      (socklen_t *)&addr_length ));
}

bool Socket::send ( const std::string s ) const {
    int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
    if ( status == -1 ) {
        return false;
    } else {
        return true;
    }
}

int Socket::recv ( std::string& s ) const {
    char buf [ MAXRECV + 1 ];
    s = "";
    memset ( buf, 0, MAXRECV + 1 );
    int status = ::recv ( m_sock, buf, MAXRECV, 0 );
    if ( status == -1 ) {
        std::cout << "status == -1   errno == " 
                  << errno << "  in Socket::recv\n";
        return 0;
    } else if ( status == 0 ) {
        return 0;
    } else {
        s = buf;
        return status;
    }
}

bool Socket::connect ( const std::string host, const int port ) {
    if ( ! is_valid() ) return false;
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons ( port );
    int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );
    if ( errno == EAFNOSUPPORT ) return false;
    status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );
    if ( status == 0 )
        return true;
    else
        return false;
}

void Socket::set_non_blocking ( const bool b ) {
    int opts;
    opts = fcntl ( m_sock, F_GETFL ); 
    if ( opts < 0 ) {
        return;
    }
    if ( b )
        opts = ( opts | O_NONBLOCK );
    else
        opts = ( opts & ~O_NONBLOCK );
    fcntl ( m_sock, F_SETFL,opts ); 
}
