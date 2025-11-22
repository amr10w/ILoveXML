#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<stack>

struct Token
{
    std::string type{}; // tag or value
    std::string text{};
    int level {};
};

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

bool is_valid_char(const char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    
    if (c >= '0' && c <= '9')
        return true;

    if (c == '-' || c == '_' || c == '.' || c == ':')
        return true;

    return false;
}

bool isValidFirstChar(const char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return true;
    return false;
}



bool verifyImproved(const std::string& tokens)
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

void highlightErrors(const std::string &content,const ErrorInfo &errors)
{
    std::istringstream stream(content);
    std::string line;
    int i=0;
    for (int l = 1; std::getline(stream, line); l++) {
        if(i < errors.count &&l==errors.lines[i])
        {
            std::cout << ">> ERROR: " << errors.descriptions[i]<<" line:"<< line <<"\n";
            i++;
        }
        else
        {
            std::cout <<line<<"\n";
        }
    }

    while(i<errors.count)
    {
        std::cout << ">> ERROR: "<< errors.descriptions[i] <<"\n";
        i++;
    }

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

void runTest(const std::string& name, const std::string& input, bool expected) {
    bool actual = verifyImproved(input);
    std::cout << "  - " << name << ": ";
    if (actual == expected) {
        std::cout << "[PASS]";
    } else {
        std::cout << "[FAIL] (Expected: " << (expected ? "true" : "false") 
                  << ", Got: " << (actual ? "true" : "false") << ")";
    }
    std::cout << " -> Input: \"" << input << "\"\n";
}


void testValidCases() {
    std::cout << "\n--- 1. Valid Cases (Expected: true) ---\n";

    runTest("Simple Nesting", "<root><child></child></root>", true);
    runTest("Empty Content", "<tag></tag>", true);
    runTest("Whitespace/Newline", "\n<t>\n <a></a> </t> ", true);
    
    // Test for attribute handling (should be skipped)
    runTest("Tag with Attributes", "<doc id=\"1\"><item name='a'></item></doc>", true);
    runTest("Tag with Complex Attributes", "<a:root data-id=\"123\" attr='test-val'>\n</a:root>", true);

    // Test for self-closing tags
    runTest("Self-Closing Tag", "<root><br/></root>", true);
    runTest("Self-Closing with space", "<root><br /></root>", true);
    runTest("Mixed Closing", "<doc><p><br/></p></doc>", true);
    
    // Test for content handling (should be skipped)
    runTest("Content Handling", "<book>Chapter 1: Hello &amp; World 123!</book>", true);
    
    // Test for valid tag name characters
    runTest("Valid Tag Name Chars", "<m-y_t:ag.123></m-y_t:ag.123>", true);
}

void testInvalidNesting() {
    std::cout << "\n--- 2. Invalid Nesting Cases (Expected: false) ---\n";

    runTest("Mismatched Order", "<A><B></A></B>", false);
    runTest("Unclosed Tag", "<root><child>", false);
    runTest("Unopened Close Tag", "</root><tag></tag>", false);
    runTest("Dangling Close Tag", "<root></root></extra>", false);
    runTest("Mismatched Tag Name", "<root></ROOT>", false);
}


void testInvalidStructure() {
    std::cout << "\n--- 3. Invalid Structure Cases (Expected: false) ---\n";

    // Structural errors
    runTest("Stray '>' at start", ">>><root></root>", false);
    runTest("Stray '>' in middle", "<root> > </root>", false);
    runTest("Stray '<' in middle", "<root><<child></child></root>", false);

    // Tag name validation errors
    runTest("Empty Tag Name", "<></>", false);
    runTest("Bad First Char (Digit)", "<1tag></1tag>", false);
    runTest("Invalid Char in Name (!)", "<tag!name></tag!name>", false);

    // Malformed tag endings
    runTest("Missing End Bracket", "<tag", false);
    runTest("Closing Tag w/ Attributes", "<root></root attr=\"val\">", false);
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

int main()
{
    std::string xmlContent = readFileToString("sample.xml");
    std::cout<<xmlContent<<"\n";
    std::cout<<verifyImproved(xmlContent)<<"\n";
    
    ErrorInfo errors =countErrorSummary(xmlContent);
    std::cout<<errors.count<<"\n";
    
    for(auto &e:errors.lines)
    {
        std::cout<<e<<"\n";
    }
    std::cout<<"\n\n";

    highlightErrors(xmlContent,errors);

    std::string out = fixXml(xmlContent,errors);
    writeToFile("out.xml",out);
    std::string xmlOut = readFileToString("out.xml");


    std::cout<<"IS Valid: "<<verifyImproved(xmlOut)<<"\n";
    
    std::cout << "Starting XML/HTML Tag Verifier Test Suite\n";

    testValidCases();
    testInvalidNesting();
    testInvalidStructure();

    std::cout << "\n--- Test Suite Complete ---\n";
    

}