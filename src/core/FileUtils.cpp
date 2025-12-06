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


std::vector<Token> tokenizeXML(const std::string& xmlContent)
{
    std::vector<Token> tokens;

    for (int i = 0; i < xmlContent.length(); i++)
    {
        if (xmlContent[i] == '<')
        {
            Token token;
            token.type = "tag";
            i++;
            while (xmlContent[i] != '>')
            {
                token.text += xmlContent[i];
                i++;
            }
            tokens.push_back(token);
        }
        else if (xmlContent[i] == '\n' || xmlContent[i] == ' ')
        {
            continue;
        }
        else
        {
            Token token;
            token.type = "value";
            while (xmlContent[i] != '<')
            {
                token.text += xmlContent[i];
                i++;
            }
            i--; // step back so the loop sees '<'
            tokens.push_back(token);
        }
    }

    return tokens;
}

Tree<std::string>* buildTree(const std::vector<Token>& tokens)
{
    Node<std::string>* root = nullptr;
    std::stack<Node<std::string>*> st;

    for (auto& e : tokens)
    {
        if (e.type == "tag")
        {
            // Opening tag: <tag>
            if (!e.text.empty() && e.text[0] != '/')
            {
                Node<std::string>* node = new Node<std::string>(e.text);

                if (!st.empty())
                {
                    Node<std::string>* parent = st.top();
                    parent->addChild(node);
                }
                else
                {
                    root = node;
                }

                st.push(node);
            }
            // Closing tag: </tag>
            else if (!e.text.empty())
            {
                st.pop();
            }
        }
        else // value
        {
            Node<std::string>* node = new Node<std::string>(e.text);
            Node<std::string>* parent = st.top();
            parent->addChild(node);
        }
    }

    return new Tree<std::string>(root);
}


