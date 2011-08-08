#include "ServerSocketListener.h"
#include "HttpRequestHandler.h"

#include <vector>
#include <pthread.h>

void* request_handler(void *param) {
    int client_local = *((int *)param);
    HttpRequestHandler handle;
    handle.perform_request(client_local);
    close(client_local);
}

int ServerSocketListener::start_listening() {
    struct sockaddr_in cliAddr; 
    int incoming;
    int addr_len;
    pthread_t thread;
    while ( 1 ) {
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
    return 0;
}
