#ifndef _BOT_SERVER_SOCKET_H_
#define _BOT_SERVER_SOCKET_H_

#include "Socket.h"

class BotServerSocket : private Socket {
public:
    BotServerSocket ( int port );
    BotServerSocket (){};
    virtual ~BotServerSocket();

    const BotServerSocket& operator << ( const std::string& ) const;
    const BotServerSocket& operator >> ( std::string& ) const;

    int accept ();

};


#endif
