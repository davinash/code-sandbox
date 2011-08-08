#include "CodeHandler.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

char CodeHandler::from_hex(char ch) {
    return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
char CodeHandler::to_hex(char code) {
    static char hex[] = "0123456789abcdef";
    return hex[code & 15];
}

char* CodeHandler::url_decode(char *str) {
    char *pstr = str, *buf = (char*)malloc(strlen(str) + 1), *pbuf = buf;
    while (*pstr) {
        if (*pstr == '%') {
            if (pstr[1] && pstr[2]) {
                *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
                pstr += 2;
            }
        } else if (*pstr == '+') { 
            *pbuf++ = ' ';
        } else {
            *pbuf++ = *pstr;
        }
        pstr++;
    }
    *pbuf = '\0';
    return buf;
}
CodeHandler::CodeHandler() {
    m_code = "";
    m_language = "";
    m_compile_output = "";
}
CodeHandler::~CodeHandler() {
}
int CodeHandler::write_code_in_temporary_file() {
    std::ofstream myfile ("code/example.cpp");
    if (myfile.is_open()) {
        myfile << m_code;
        myfile.close();
    }
    return 0;
}
int CodeHandler::compile_code() {
    FILE *pipe = popen ( m_compile_command.c_str(), "r");
    if (!pipe ) return -1;
    char buffer[128];
    while ( !feof(pipe)){
        if ( fgets ( buffer, 128, pipe) != NULL ) {
            m_compile_output += buffer;
        }
    }
    pclose ( pipe );
    //std::cout << "OUTPUT :" << std::endl;
    //std::cout << m_compile_output << std::endl;
    return 0;
}
void CodeHandler::prepare_compile_command() {
    m_compile_command = "g++ -c code/example.cpp";
}

void CodeHandler::perform_operation(std::string &szCode, 
                                    std::string &szLang) {
    m_language = szLang;
    m_code = url_decode((char*)szCode.c_str());
    write_code_in_temporary_file();
    prepare_compile_command();
    compile_code();
}

