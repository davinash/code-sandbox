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
#include "HttpRequestHandler.h"
#include "CodeHandler.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <iterator>
#include <fstream>
#include <sstream>

HttpRequestHandler::HttpRequestHandler(){
}
HttpRequestHandler::~HttpRequestHandler(){
}
void HttpRequestHandler::perform_request( int sockfd) {
    m_filename = "";
    m_localfd  = sockfd;
    m_urlParms.clear();
    m_postParms.clear();
    m_requestHeader.clear();
    m_replaceToken.clear();
    m_contentlength=0;

    std::string header;
    std::string message;

    int rc = read_header(header, message);
    if ( rc != HTTP_ERR_OK ) {
        return;
    }
    rc = parse_header(header);
    if ( rc != HTTP_ERR_OK ) {
        return;
    }
    rc = parse_message(message);
    if ( rc != HTTP_ERR_OK ){
        return;
    }
    if ( !m_filename.length()) {
    } else {
        handle_file_requested();
    }
    write_response();
}
int HttpRequestHandler::handle_file_requested() {
    std::ifstream ifs("htdocs/init_form.html");
    std::string str((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
    m_Content = str;
    ifs.close();
}
int HttpRequestHandler::write_response(int header ) {
    std::ostringstream response;
    response << "HTTP/1.1";
    response << " " << "200 OK" << "\n";
    if ( m_Content.length())
        response << "Length: " << m_Content.length() << "\n";
    response << "Content-Type: " << "text/html" << "\n";

    std::string response_s = response.str();

    int rc = write(m_localfd, response_s.data(), response_s.length());
    rc = write(m_localfd, m_Content.data(), m_Content.length());

    return rc;
}

int HttpRequestHandler::parse_message( std::string &message) {
    if( !m_contentlength) return HTTP_ERR_OK;
    if ( m_contentlength < message.length()){
        message = message.substr(0, m_contentlength);
    } else if  ( m_contentlength > message.length()) {
        while ( m_contentlength > message.length()) {
            m_inputBuffer[0] = 0;
            int rc = read(m_localfd,
                          &m_inputBuffer[0],
                          INPUT_BUFFER_SIZE);
            if ( rc < 0 ){
                return HTTP_ERR_GENERIC;
            }
            message += m_inputBuffer;
        }
        if( m_contentlength < message.length() ) {
            message = message.substr(0, m_contentlength);
        }
    }
    parse_message_pairs(message);
    CodeHandler codeHandle;
    /*key_value_t::itr code = m_postParms.find("Code");
    if ( code != key_value_t::npos) {
        key_value_t::itr lang = m_postParms.find("lang");
    }*/

    codeHandle.perform_operation(m_postParms["code"],m_postParms["lang"]);
}
int HttpRequestHandler::parse_message_pairs(std::string &message) {
    std::string id;
    std::string value;
    int state = 0;
    for(unsigned int i=0; i < message.length();) {
        switch( state ) {
        case 0:
            id     ="";
            value  ="";
            state  =1;
            break;
        case 1:
            switch( message[i] ) {
            case '=':
                state=2;
                break;
            default:
                id += message[i];
                break;
            }
            i++;
            break;
        case 2:
            switch( message[i] ) {
            case '&':
                m_postParms[id] = value;
                state = 0;
                break;
            default:
                value += message[i];
                break;
            }
            i++;
            break;
        }
    }
    if( state == 2 )
        m_postParms[id]=value;
    return HTTP_ERR_OK;
}

int HttpRequestHandler::read_header( std::string &header, 
                                     std::string &message) {
    header = "";
    unsigned int offset = 0;
    while ((offset = header.find("\r\n\r\n")) == std::string::npos ) {
        m_inputBuffer[0]=0;
        int r = read(m_localfd, &m_inputBuffer[0],INPUT_BUFFER_SIZE);
        if ( r < 0 ) {
            return -1;
        }
        m_inputBuffer[r]=0;
        header += m_inputBuffer;
    }
    message = header.substr(offset + 4);
    /* Fix the case where only "GET /xxxxx HTTP1.0\r\n\r\n" is sent (no
     * other headers)*/
    if(offset == header.find("\r\n")) {
        header = header.substr(0,offset + 2);
    } else {
        header = header.substr(0,offset);
    }
    return 0;
}
int HttpRequestHandler::parse_header( std::string &header) {
    char *request       = NULL;
    char *request_end   = NULL;
    char *pHeader = (char*)header.c_str();
    m_filename = "";
    m_contentlength = 0;
    m_requesttype = -1;

    request_end = strstr(pHeader," HTTP/1.1\r\n");
    if ( !request_end)  {
        return -1;
    }
    if ( m_requesttype == -1 ) {
        request = strstr ( pHeader, "GET ");
        if ( request ) {
            request += 4;
            m_requesttype = HTTP_REQUEST_GET; 
        }
    }
    if ( m_requesttype == -1 ){
        request = strstr ( pHeader, "POST ");
        if ( request ) {
            request += 5;
            m_requesttype = HTTP_REQUEST_POST; 
        }
    }
    if ( m_requesttype == -1 ) {
        // did not find either GET or POST
        return HTTP_ERR_GENERIC;
    }
    if ( request_end <= request ) {
        // malformed request
        return HTTP_ERR_GENERIC;
    }
    while ( request != request_end ) {
        // get the url requested
        m_filename += *request++;
    }
    // Move to the end of the requst
    request_end += 11;
    while ( *request_end != '\r' && *request_end != '\0' ) {
        char *keyend = strstr(request_end,": ");
        // get the key
        if ( keyend && keyend > request_end ) {
            std::string key, value;
            key   = "";
            value = "";
            while ( keyend > request_end)
                key += (*request_end++);
            // Get the value of the key
            request_end = keyend + 2;
            char *valueend = strstr ( request_end, "\r\n");
            // are we at the end of header section
            if ( !valueend)
                valueend = request_end + strlen ( request_end );
            if ( valueend ) {
                while ( request_end < valueend)
                    value += *request_end++;
                // add key value pair to map
                m_requestHeader[key] = value;
                if (*request_end)
                    request_end += 2;
            } else {
                return HTTP_ERR_GENERIC;
            }
        }
    } /* while loop ends here */
    // find the content length
    m_contentlength = atoi ( m_requestHeader["Content-Length"].c_str());
    std::cout << "Content-Length : " << m_contentlength << std::endl;
    return 0;
}

