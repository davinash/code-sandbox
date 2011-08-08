#ifndef _SOCKET_EXCEPTION_H_
#define _SOCKET_EXCEPTION_H_

#include <string>

class SocketException {
public:
    SocketException ( std::string in_szDesc ) : 
        m_szDescription ( in_szDesc  ) {};
    ~SocketException (){};
    std::string description() { 
        return m_szDescription; 
    }
private:
    std::string m_szDescription;
};
#endif
