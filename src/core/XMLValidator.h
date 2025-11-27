#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H
#include <iostream>
#include <vector>

struct ErrorInfo
{
    int count {};
    std::vector<int> lines{};
    std::vector<std::string> descriptions{};
    std::vector<std::string> types{};
    std::vector<std::string> tags{};
    std::vector<int> levels{};

    
    void addError(int line, int level, 
              const std::string& tag, 
              const std::string& type, 
              const std::string& desc)
    {
        count++;
        lines.push_back(line);
        levels.push_back(level);
        tags.push_back(tag);
        types.push_back(type);
        descriptions.push_back(desc);
    }

};


bool verify(const std::string& tokens);
bool is_valid_char(const char c);
bool isValidFirstChar(const char c);
bool isClosingTag(const std::string & tag);
std::string getTagName(const std::string &tagText);
ErrorInfo countErrorSummary(const std::string &content);
std::string fixXml(const std::string & content,const ErrorInfo &errors );
#endif