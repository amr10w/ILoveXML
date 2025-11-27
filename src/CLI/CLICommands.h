#include "FileUtils.h"
#include<XMLValidator.h>
#include <vector>

enum ErrorCode {
    OK = 0,
    ERR_UNKNOWN_COMMAND = 1,
    ERR_MISSING_ARGUMENT = 2,
    ERR_INVALID_OPTION = 3,
    ERR_FILE_NOT_FOUND = 4,
};
class CLICommands
{
public:
    int handle(int argc, char* argv[]);
private:
    int verifyCommand(const std::vector<std::string>& args);
};