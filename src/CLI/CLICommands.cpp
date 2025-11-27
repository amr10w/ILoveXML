#include "CLICommands.h"

int CLICommands::handle(int argc, char *argv[])
{
     if(argc < 2) {
        std::cout << "Usage: XNet <command> [options]\n";
        return ERR_MISSING_ARGUMENT;
    }

    std::string cmd = argv[1];
    std::vector<std::string> args(argv + 2, argv + argc);

    if(cmd == "verify") return verifyCommand(args);

    std::cerr << "Unknown command: " << cmd << "\n";
    return ERR_UNKNOWN_COMMAND;

}

int CLICommands::verifyCommand(const std::vector<std::string> &args)
{
    if(args.size() < 2 || args[0] != "-i") {
        std::cout << "Usage: verify -i <filename>\n";
        return ERR_INVALID_OPTION;
    }

    std::string filename = args[1];
    std::string content = readFileToString(filename);
    if(content=="") return ERR_FILE_NOT_FOUND;
    bool result = verify(content);

    if(result)
    {
        std::cout << "Verification succeeded.\n";
        return OK;
    }
    else{
        std::cout << "Verification failed.\n";
    }

    ErrorInfo errors = countErrorSummary(content);
    std::cout<<"Number Of Errors: "<<errors.count<<"\n";
    
    for(int i=0;i<errors.lines.size();i++)
    {
        std::cout<<errors.lines[i]<<" "<<errors.descriptions[i]<<"\n";
    }
    

    if(!result&&args.size()> 2 )
    {
        if(args[2] == "-f" && args[3] == "-o")
        {
            std::string out =fixXml(content,errors);
            writeToFile(args[4],out);
            return OK;
        }
        else
        {
            std::cerr << "Usage: verify -i <filename> -f -o <filename>\n";
            return ERR_INVALID_OPTION;
        }
    }
    
    return OK;
}
