#ifndef _CODE_HANDLER_H_
#define _CODE_HANDLER_H_

#include <string>
#include <map>
class CodeHandler {
public:
    CodeHandler();
    ~CodeHandler();
    void perform_operation(std::string &szCode, std::string &szLang);
private:
    std::string m_language;
    std::string m_code;
    std::string m_compile_output;
    std::string m_compile_command;
    std::map<std::string, std::string> m_urlMap;
    void DecodeUri(std::string &inSzSource);
    char from_hex(char ch);
    char to_hex(char code);
    char* url_decode(char *str); 
    int write_code_in_temporary_file();
    int compile_code();
    void prepare_compile_command(); 
};
#endif /* _CODE_HANDLER_H_*/
