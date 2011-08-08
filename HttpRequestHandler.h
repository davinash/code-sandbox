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
#ifndef _HTTP_REQUEST_HANDLER_H_
#define _HTTP_REQUEST_HANDLER_H_

#include <string>
#include <map>

#define INPUT_BUFFER_SIZE   10240

enum{
    HTTP_ERR_GENERIC = -1,
    HTTP_ERR_OK,
    HTTP_ERR_CONNECTLOST,
    HTTP_ERR_APP,
    HTTP_ERR_REGEX,
    HTTP_HDR_NOTHING, 
    HTTP_HDR_OK,
    HTTP_BINARY_FILE,
    HTTP_TEXT_FILE,  
    HTTP_REQUEST_GET, 
    HTTP_REQUEST_POST,
    HTTP_LENGTH_REQUIRED
};

typedef std::map<std::string, std::string> key_value_t;

class HttpRequestHandler {
public:
    HttpRequestHandler();
    ~HttpRequestHandler();
    void perform_request(int sockfd);
private:
    char m_inputBuffer[INPUT_BUFFER_SIZE];
    std::string m_filename; // filename of the URL.
    std::string m_Content; // response output buffer.
    int m_localfd;
    key_value_t m_urlParms;
    key_value_t m_postParms;
    key_value_t m_requestHeader;
    key_value_t m_replaceToken;
    key_value_t m_ResponseHeader;
    unsigned int m_contentlength;
    int m_requesttype;

    struct http_response_data {
        std::string content_type;
        std::string content_encoding;
        unsigned int length;
        int status;
        http_response_data() {
            length = 0;
            status = 0;
        }
    };
    int read_header(std::string &, std::string &);
    int parse_header(std::string &);
    int parse_message( std::string &);
    int handle_file_requested();
    int write_response(int header=HTTP_HDR_OK);
    int parse_message_pairs( std::string &);

};

#endif


