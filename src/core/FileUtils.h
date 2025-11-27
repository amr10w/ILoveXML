#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <string>
#include <vector>

struct Token
{
    std::string type{}; // tag or value
    std::string text{};
    int level {};
};

std::string readFileToString(const std::string& filename);
void writeToFile(const std::string& filename, const std::string& content);  
std::vector<Token> tokenizeLine(const std::string& line);

#endif