#ifndef _SANDBOX_CONTEXT_H_
#define _SANDBOX_CONTEXT_H_

#include <string>
#include <map>

#include "ServerSocket.h"
#include "ServerSocketListener.h"

class SandboxContext {
public:
    // CTOR
    SandboxContext();
    ~SandboxContext();
    int init_context( int, char**);

    ServerSocket *m_server_socket;
    ServerSocketListener *m_socket_listner;
private:
    char* get_command_options( char**, char **, const std::string &);

    std::map<std::string, std::string> m_lang_compiler_map;
    int m_port_num;


};
#endif /* _SANDBOX_CONTEXT_H_*/

