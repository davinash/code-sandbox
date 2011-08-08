#include "BotServerSocket.h"
#include "SocketException.h"

BotServerSocket::BotServerSocket ( int port ) {
    if ( ! Socket::create() ) {
        throw SocketException ( "Could not create server socket." );
    }
    if ( ! Socket::bind ( port ) ) {
        throw SocketException ( "Could not bind to port." );
    }
    if ( ! Socket::listen() ) {
        throw SocketException ( "Could not listen to socket." );
    }
}

BotServerSocket::~BotServerSocket() { }
const BotServerSocket& BotServerSocket::
                operator << ( const std::string& s ) const { 
    if ( ! Socket::send ( s ) ) {
        throw SocketException ( "Could not write to socket." );
    }
    return *this;
}

const BotServerSocket& BotServerSocket::operator >> ( std::string& s ) const {
    if ( ! Socket::recv ( s ) ) {
        throw SocketException ( "Could not read from socket." );
    }
    return *this;
}

int BotServerSocket::accept () {
    return Socket::accept ();
}

