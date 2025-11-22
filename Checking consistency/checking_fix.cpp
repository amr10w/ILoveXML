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
/*
the std::string::find() method is used to locate the first occurrence
of a substring or character within a string. This function returns 
the starting index of the found substring or character. 
If the substring or character is not found, it returns std::string::npos
*/
std::vector<Token> tokenizeLine(const std::string& line) {
    std::vector<Token> tokens;
    int i = 0;
    while (i < (int)line.size()) {
        int numSpace = 1;
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




bool verify(const std::string& content)
{
    std::stack<std::string> st;
    
    
    for(int i=0;i<(int)content.size();i++)
    {
        char c=content[i];
        if(c=='<')
        {
            bool closed=false;
            std::string tag="";
            i++;
            if(c=='/'&&st.empty())
                return false;
            else if(c=='/')
            {    
                closed =true;
                i++;
            }

            // check for first char (can't be digit)
            if(!isValidFirstChar(c))
                return false;
            
            while(i < content.size() && c != '>')
            {
                if(!is_valid_char(c))
                    return false;
                tag+=c;
                i++;
            }

            if(closed&&tag==st.top())
                st.pop();
            else if(closed)
                return false;
            else
                st.push(tag);

        }
        else if(c=='\n'||c==' ')
            continue;
        else if(c=='>')
            return false;
            
    }
    
    return st.empty() ? true:false;
}

bool verify_improved(const std::string& tokens)
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
struct Errors
{
    int Count {};
    std::vector<int> Lines{};
    std::vector<std::string> describe{};
    std::vector<std::string> types{};
    std::vector<std::string> tags{};
    std::vector<int> levels{};
};

Errors countErrorSummaryV2(const std::string &content)
{
    int errorCount = 0;
    std::vector<int> errorLines;
    std::vector<std::string> describes;
    std::vector<std::string> types_error;
    std::vector<int> levels_error;
    std::vector<std::string> tags_error;
    std::stack<Token> tagStack;
    std::string line;
    std::istringstream stream(content);
    int l=1;
    while(std::getline(stream, line)) {
        

        
        std::vector<Token> tokens = tokenizeLine(line);
        for(auto &e:tokens)
        {
            std::cout<<e.type<<": "<<e.text<<", "<<e.level<<"\n";
        }
        std::cout<<"\n\n";

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
                        if(tagStack.top().text==getTagName(token.text))
                        {
                            errorCount++;
                            errorLines.push_back(l-1);
                            std::string s="The missing closing for <" +cause+">";
                            describes.push_back(s);
                            types_error.push_back("Missing");
                            levels_error.push_back(level);
                            tags_error.push_back(cause);
                            tagStack.pop();
                        }
                        else
                        {
                            errorCount++;
                            errorLines.push_back(l);
                            std::string s="The mismatching closing for <" +cause+">";
                            types_error.push_back("Mismatched");
                            describes.push_back(s);
                            levels_error.push_back(level); 
                            tags_error.push_back(cause);
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
                        errorCount++;
                        errorLines.push_back(l-1);
                        std::string s="The missing closing for <" + tagStack.top().text+">";
                        describes.push_back(s);
                        types_error.push_back("Missing");
                        levels_error.push_back(tagStack.top().level);
                        tags_error.push_back(tagStack.top().text);
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
        errorCount++;
        errorLines.push_back(l);
        std::string s="The missing closing for <" +tagStack.top().text+">";
        types_error.push_back("Missing");
        describes.push_back(s);
        levels_error.push_back(tagStack.top().level);
        tags_error.push_back(tagStack.top().text);
        tagStack.pop();

    }

    Errors errors;
    errors.Count=errorCount;
    errors.Lines=errorLines;
    errors.describe=describes;
    errors.types=types_error;
    errors.levels=levels_error;
    errors.tags=tags_error;
    return errors;

}

Errors countErrorSummary(const std::string &content)
{
    
    
    int errorCount = 0;
    std::vector<int> errorLines;
    std::stack<std::string> tagStack;
    std::string line;
    std::istringstream stream(content);
    
    for (int l = 1; std::getline(stream, line); l++) {
        

        
        std::vector<Token> tokens = tokenizeLine(line);

        for(auto &token:tokens)
        {
            if(token.type=="tag")
            {
                if(isClosingTag(token.text))
                {
                    if(tagStack.empty()|| tagStack.top()!=getTagName(token.text))
                    {
                        errorCount++;
                        errorLines.push_back(l);
                    }
                    else
                    {
                        tagStack.pop();
                    }
                    
                }
                else
                {
                    tagStack.push(getTagName(token.text));
                }
            }
        }
    }

    while(!tagStack.empty())
    {
        errorCount++;
        errorLines.push_back(0);
        tagStack.pop();

    }

    Errors errors;
    errors.Count=errorCount;
    errors.Lines=errorLines;
    return errors;

}

// Updated struct to store line number with the tag name
struct TagInfo {
    std::string name;
    int line;
};

// Updated error struct for clarity
struct ErrorSummary {
    int count = 0;
    std::vector<std::pair<int, std::string>> errors; // Pair: <Line Number, Error Description>
};
ErrorSummary countErrorSummary_Improved(const std::string& content)
{
    ErrorSummary summary;
    std::stack<TagInfo> tagStack;

    int currentLine = 1;
    for (int i = 0; i < (int)content.size(); )
    {
        char c = content[i];

        // 1. Handle Whitespace and Newlines
        if (c == '\n') {
            currentLine++;
            i++;
            continue;
        }

        if (isspace(c)) {
            i++;
            continue;
        }

        // 2. Start of Tag Processing
        if (c == '<')
        {
            int tag_start_line = currentLine;
            i++; // Move past '<'

            if (i >= content.size()) {
                summary.count++;
                summary.errors.push_back({tag_start_line, "Malformed Tag: Document ends abruptly after '<'."});
                break; 
            }

            bool is_closing = false;
            if (content[i] == '/')
            {
                is_closing = true;
                i++; // Move past '/'
            }
            
            // 2a. Validate Tag Name Start
            if (i >= content.size() || !isValidFirstChar(content[i])) {
                // Handle empty tags like <>, </>, or tags starting with invalid char like <1tag>
                summary.count++;
                summary.errors.push_back({tag_start_line, "Malformed Tag: Tag name is missing or starts with an invalid character."});
                
                // Recovery: Skip everything until the next '<' or '>'
                while (i < content.size() && content[i] != '>' && content[i] != '<') {
                    if (content[i] == '\n') currentLine++;
                    i++;
                }
                if (i < content.size() && content[i] == '>') i++; // Consume '>' if found
                
                continue; 
            }

            // 2b. Extract Tag Name
            std::string tag = "";
            while (i < content.size() && content[i] != '>' && !isspace(content[i]))
            {
                if (!is_valid_char(content[i])) {
                    summary.count++;
                    summary.errors.push_back({tag_start_line, "Malformed Tag: Invalid character in tag name: '" + std::string(1, content[i]) + "'."});
                    
                    // Recovery for invalid char inside name: Skip to the end of this token
                    while (i < content.size() && is_valid_char(content[i])) i++;
                    // Fall through to the next recovery step below
                    break;
                }
                tag += content[i];
                i++;
            }

            // 2c. Skip Attributes and Find End of Tag ('>' or '/>')
            bool is_self_closing = false;
            
            while (i < content.size() && content[i] != '>')
            {
                if (content[i] == '\n') currentLine++;
                
                // CRITICAL RECOVERY CHECK: Found '<' before finding the closing '>'
                if (content[i] == '<') {
                    summary.count++;
                    summary.errors.push_back({tag_start_line, "Structural Error: Malformed tag. Missing closing '>' for <" + tag + ">."});
                    // i is correctly positioned at the next '<'. We break and continue the main loop.
                    goto end_tag_processing;
                }

                // Check for self-closing sequence (only allowed in opening tags)
                if (!is_closing && content[i] == '/' && i + 1 < content.size() && content[i + 1] == '>')
                {
                    is_self_closing = true;
                    i++; // move to '>'
                    break;
                }
                i++;
            }
            
            // 2d. Final Tag Termination Check
            if (i >= content.size() || content[i] != '>') {
                summary.count++;
                summary.errors.push_back({tag_start_line, "Malformed Tag: Missing closing '>'."});
                
                // Final file end recovery: Move pointer to the end of the file.
                while (i < content.size()) {
                    if (content[i] == '\n') currentLine++;
                    i++;
                }
                goto end_tag_processing;
            }
            
            // 3. Core Stack Logic
            if (is_closing)
            {
                if (tagStack.empty() || tagStack.top().name != tag)
                {
                    summary.count++;
                    summary.errors.push_back({tag_start_line, "Mismatched Closing Tag: Encountered </" + tag + ">, but expected </" + (tagStack.empty() ? "(none)" : tagStack.top().name) + ">."});
                }
                else
                {
                    tagStack.pop();
                }
            }
            else if (is_self_closing)
            {
                // Self-closing tag, do nothing to the stack.
            }
            else
            {
                // Opening tag
                tagStack.push({tag, tag_start_line});
            }

            i++; // Move past the final '>'

            end_tag_processing:; // Label for error recovery jumps
        }
        else if (c == '>') // 4. Stray '>' character
        {
            summary.count++;
            summary.errors.push_back({currentLine, "Structural Error: Stray '>' character found outside of a tag."});
            i++;
        }
        else // 5. Content Block
        {
            // Skip content until next '<'
            while (i < content.size() && content[i] != '<')
            {
                if (content[i] == '\n') currentLine++;
                i++;
            }
        }
    }

    // 6. Process Unclosed Tags remaining on the stack
    while(!tagStack.empty())
    {
        summary.count++;
        // Use the line number where the tag was opened (TagInfo::line)
        summary.errors.push_back({tagStack.top().line, "Unclosed Tag: The <" + tagStack.top().name + "> tag, opened on this line, was never closed."});
        tagStack.pop();
    }

    return summary;
}

void highlighting_Errors(const std::string content,const Errors &errors)
{
    std::istringstream stream(content);
    std::string line;
    int i=0;
    for (int l = 1; std::getline(stream, line); l++) {
        if(i < errors.Count &&l==errors.Lines[i])
        {
            std::cout << ">> ERROR: " << errors.describe[i]<<" line:"<< line <<"\n";
            i++;
        }
        else
        {
            std::cout <<line<<"\n";
        }
    }

    while(i<errors.Count)
    {
        std::cout << ">> ERROR: "<< errors.describe[i] <<"\n";
        i++;
    }

}

std::string fix(const std::string & content,const Errors &errors )
{
    std::string ouyputXML="";
    std::istringstream stream(content);
    std::string line;
    int i=0;
    for (int l = 1; std::getline(stream, line); l++) {
        if(i < errors.Count && l==errors.Lines[i])
        {
            if(errors.tags[i]!="id" && errors.tags[i]!="name")
            {
                for(int j=0;j<errors.levels[i]*4;j++)
                {
                    ouyputXML+=" ";
                }
                ouyputXML+="</"+errors.tags[i]+">\n";
                
            }
            else
            {
                if(errors.types[i] == "Mismatched")
                {
                    int x;
                    for(int j=0;line[j]!='>';j++)
                    {
                        ouyputXML+=line[j];
                        x=j;
                    }
                    for(int j=x+1;line[j]!='<';j++)
                    {
                        ouyputXML+=line[j];
                    }
                    ouyputXML+="</"+errors.tags[i]+">\n";
                }
                else 
                    ouyputXML+=line+="</"+errors.tags[i]+">\n";
            }
            i++;
        }
        else
        {
            ouyputXML+=line +"\n";
        }
    }

     while(i<errors.Count)
    {
        for(int j=0;j<errors.levels[i]*4;j++)
        {
            ouyputXML+=" ";
        }
        ouyputXML+="</"+errors.tags[i]+">";
        i++;
    }
    return ouyputXML;

}

void runTest(const std::string& name, const std::string& input, bool expected) {
    bool actual = verify(input);
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
    std::cout<<verify_improved(xmlContent)<<"\n";
    
    Errors errors =countErrorSummaryV2(xmlContent);
    std::cout<<errors.Count<<"\n";
    
    for(auto &e:errors.Lines)
    {
        std::cout<<e<<"\n";
    }
    std::cout<<"\n\n";

    highlighting_Errors(xmlContent,errors);

    std::string out = fix(xmlContent,errors);
    std::cout<<out<<"\n";

    writeToFile("out.xml",out);

//    ErrorSummary error=countErrorSummary_Improved(xmlContent);
//    std::cout<<error.count<<"\n";
    
//     for(auto &e:error.errors)
//     {
//         std::cout<<e.first<<": "<<e.second<<"\n";
//     }
   
    
    // std::cout << "Starting XML/HTML Tag Verifier Test Suite\n";

    // testValidCases();
    // testInvalidNesting();
    // testInvalidStructure();

    // std::cout << "\n--- Test Suite Complete ---\n";
    

}