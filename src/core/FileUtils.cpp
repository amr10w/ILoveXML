#include "FileUtils.h"
#include <iostream>
#include <fstream>
#include<sstream>



std::string readFileToString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open the file: " << filename << std::endl;
        return "";  // return empty string on error
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

void writeToFile(const std::string& filename, const std::string& content)
{
    std::ofstream output(filename);

    if (!output.is_open())
    {
        std::cerr << "Error: Cannot open file for writing: " << filename << "\n";
        return;
    }

    output << content;  
    output.close();     
}

std::vector<Token> tokenizeLine(const std::string& line) {
    std::vector<Token> tokens;
    int i = 0;
    while (i < (int)line.size()) {
        int numSpace = 0;
        while (i < (int)line.size() && line[i] == ' ') {
            numSpace++;
            i++;
        }
        if (i < (int)line.size() && line[i] == '<') {
            Token token;
            token.level=numSpace/4;
            token.type = "tag";
            i++; 
            while (i < (int)line.size() && line[i] != '>') {
                if (line[i] == '<') break; 
                token.text += line[i++];
            }
            if (i < (int)line.size() && line[i] == '>') i++;
            tokens.push_back(token);
        } else if (!isspace(line[i])) {
            Token token;
            token.type = "value";
            while (i < (int)line.size() && line[i] != '<') {
                token.text += line[i++];
            }
            tokens.push_back(token);
        } else {
            i++;
        }
    }
    return tokens;
}



