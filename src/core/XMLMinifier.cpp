#include "XMLMinifier.h"

std::string minifyXML(const std::string& xmlContent) {
    std::string result;
    bool insideTag = false;    // Track if we are inside <...>
    bool prevSpace = false;    // Avoid multiple consecutive spaces

    for (size_t i = 0; i < xmlContent.size(); ++i) {
        char c = xmlContent[i];

        if (c == '<') {
            insideTag = true;
            // Remove trailing space before tag
            while (!result.empty() && result.back() == ' ')
                result.pop_back();
            result += c;
        }
        else if (c == '>') {
            insideTag = false;
            result += c;
        }
        else if (insideTag) {
            // Inside tag: preserve everything except newlines/tabs
            if (c != '\n' && c != '\r' && c != '\t')
                result += c;
        }
        else {
            // Outside tag (text content)
            if (!isspace(c)) {
                result += c;
                prevSpace = false;
            }
            else if (!prevSpace) {
                // Collapse multiple spaces into one
                result += ' ';
                prevSpace = true;
            }
        }
    }
    return result;
}