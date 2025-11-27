#include<XMLValidator.h>
#include "FileUtils.h"
#include<iostream> 
#include<stack> 
#include<sstream>

bool verify(const std::string& tokens)
{
    std::stack<std::string> st;
    int i = 0;
    while (i < (int)tokens.size())
    {
        char c = tokens[i];
        if (c == '<')
        {
            i++; 
            if (i >= tokens.size()) return false;
            bool is_closing = false;
            if (tokens[i] == '/')
            {
                is_closing = true;
                i++; 
                if (st.empty()) return false; 
            }
            if (i >= tokens.size() || !isValidFirstChar(tokens[i]))
                return false;
            std::string tag = "";
            while (i < tokens.size() && tokens[i] != '>' && tokens[i] != '/' && tokens[i] != ' ' && tokens[i] != '\n')
            {
                if (!is_valid_char(tokens[i]))
                    return false; // Invalid char in tag name
                tag += tokens[i];
                i++;
            }

            // 2. Skip Attributes and Find End of Tag ('>' or '/>')
            bool is_self_closing = false;

            // CASE 1: Closing tag -> attributes NOT allowed
            if (is_closing)
            {
                // Only whitespace is allowed before '>'
                while (i < tokens.size() && (tokens[i] == ' ' || tokens[i] == '\n'))
                    i++;

                // If anything appears before '>', this is invalid
                if (i >= tokens.size() || tokens[i] != '>')
                    return false;
            }
            else
            {
                // CASE 2: Opening tag -> skip attributes, detect '/>'
                while (i < tokens.size() && tokens[i] != '>')
                {
                    if (tokens[i] == '/' && i + 1 < tokens.size() && tokens[i + 1] == '>')
                    {
                        is_self_closing = true;
                        i++; // move to '>'
                        break;
                    }
                    i++;
                }
            }
            
            if (i >= tokens.size() || tokens[i] != '>')
                return false; // Missing closing '>'

            // last step
            if (is_closing)
            {
                if (st.top() == tag)
                    st.pop();
                else
                    return false; // Mismatched closing tag
            }
            else if (is_self_closing)
            {
                // Self-closing tag (e.g., <br /> or <img />). Do nothing to the stack.
                // Note: If you want to support <tag/> instead of <tag />, you would modify the skip loop.
            }
            else
            {
                st.push(tag); // Opening tag
            }

            i++; // Move past '>'
        }
        else if (c == '\n' || c == ' ')
        {
            i++; // Skip whitespace
        }
        else if (c == '>')
        {
            return false; // Stray '>' character outside of a tag sequence
        }
        else
        {
            // CONTENT BLOCK (Text, entities, etc.)
            // We can safely skip all content until we find the next '<'
            while (i < tokens.size() && tokens[i] != '<')
            {
                i++;
            }
            // The loop will continue, and the next char will be handled in the next iteration.
        }
    }
    return st.empty();
}

// helper function 1

bool is_valid_char(const char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    
    if (c >= '0' && c <= '9')
        return true;

    if (c == '-' || c == '_' || c == '.' || c == ':')
        return true;

    return false;
}

// helper function 2
bool isValidFirstChar(const char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    return false;
}



bool isClosingTag(const std::string & tag)
{
    return !tag.empty() && tag[0] == '/';
}

std::string getTagName(const std::string &tagText)
{
    int i=0;
    if (tagText[i] == '/') i++;

    std::string tagName = "";
    while(i<tagText.length() &&tagText[i]!=' '&& tagText[i] != '/' && tagText[i] != '>' )
    {
        tagName+=tagText[i];
        i++;
    }
    return tagName;
}


ErrorInfo countErrorSummary(const std::string &content)
{

    ErrorInfo errors;
    std::string line;
    std::istringstream stream(content);
    std::stack<Token> tagStack;
    int l=1;
    while(std::getline(stream, line)) {
        

        
        std::vector<Token> tokens = tokenizeLine(line);
        for(auto &token:tokens)
        {
            if(token.type=="tag")
            {
                if(isClosingTag(token.text))
                {
                    if(tagStack.empty()|| tagStack.top().text!=getTagName(token.text) )
                    {
                        std::string cause=tagStack.top().text;
                        int level=tagStack.top().level;
                        tagStack.pop();
                        if(!tagStack.empty()&&tagStack.top().text==getTagName(token.text))
                        {
                            
                            std::string s="The missing closing for <" +cause+">";
                            errors.addError(l-1,level,cause,"Missing",s);
                            tagStack.pop();
                        }
                        else
                        {
                            std::string s="The mismatching closing for <" +cause+">";
                            errors.addError(l,level,cause,"Mismatched",s);
                        }
                    }
                    else
                    {
                        tagStack.pop();
                    }
                    
                }
                else
                {
                    if(!tagStack.empty()&&tagStack.top().level==token.level)
                    {
                        std::string s="The missing closing for <" + tagStack.top().text+">";
                        errors.addError(l-1,tagStack.top().level,tagStack.top().text,"Missing",s);
                        tagStack.pop();
                    }
                    
                    tagStack.push(token);
                }
            }
        }
        l++;
    }

    while(!tagStack.empty())
    {
        std::string s="The missing closing for <" +tagStack.top().text+">";
        errors.addError(l,tagStack.top().level,tagStack.top().text,"Missing",s);
        tagStack.pop();

    }
    return errors;

}


std::string fixXml(const std::string & content,const ErrorInfo &errors )
{
    std::string outputXML="";
    std::istringstream stream(content);
    std::string line;
    int i=0;
    for (int l = 1; std::getline(stream, line); l++) {
        if(i < errors.count && l==errors.lines[i])
        {
            if(errors.tags[i]!="id" && errors.tags[i]!="name")
            {
                for(int j=0;j<errors.levels[i]*4;j++)
                {
                    outputXML+=" ";
                }
                outputXML+="</"+errors.tags[i]+">\n";
                
            }
            else
            {
                if(errors.types[i] == "Mismatched")
                {
                    int x;
                    for(int j=0;line[j]!='>';j++)
                    {
                        outputXML+=line[j];
                        x=j;
                    }
                    for(int j=x+1;line[j]!='<';j++)
                    {
                        outputXML+=line[j];
                    }
                    outputXML+="</"+errors.tags[i]+">\n";
                }
                else 
                    outputXML+=line + "</"+errors.tags[i]+">\n";
            }
            i++;
        }
        else
        {
            outputXML+=line +"\n";
        }
    }

     while(i<errors.count)
    {
        for(int j=0;j<errors.levels[i]*4;j++)
        {
            outputXML+=" ";
        }
        outputXML+="</"+errors.tags[i]+">";
        i++;
    }
    return outputXML;

}


