#include "SandboxContext.h"
#include "ServerSocketListener.h"

#include <algorithm>
SandboxContext::SandboxContext() {
}
SandboxContext::~SandboxContext() {
    m_lang_compiler_map.clear();
    delete m_server_socket;
    delete m_socket_listner;
}
int SandboxContext::init_context( int argc, char **argv ) {
    int rc = 0;
    //Initialize language to compiler
    //TODO: 
    //This should be done through configuration file.
    m_lang_compiler_map["cpp"]  = "g++";
    m_lang_compiler_map["c"]    = "gcc";
    m_lang_compiler_map["perl"] = "perl";
    m_lang_compiler_map["java"] = "javac";
    // Create object of server socket
    m_server_socket = new ServerSocket();
    // Create listener object
    m_socket_listner = new ServerSocketListener();
    return rc;
}
char* SandboxContext::get_command_options( char** begin, char** end, 
                                           const std::string &option){
    char **itr  = std::find(begin, end, option);
    if ( itr != end && ++itr != end ) {
        return *itr;
    }
    return 0;
}
