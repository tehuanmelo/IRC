#include "../inc/parsing.hpp"
#include "../inc/utils.hpp"



bool isValidParam(std::string param)
{
    std::string invalidParam = "\n\r\t:";
    size_t found = param.find_first_of(invalidParam);
    return found == std::string::npos;
}

ircMessage parseMessage(std::string message)
{
    if (message.empty()) {
        ircMessage emptyMessage;
        return emptyMessage;
    }

    ircMessage parsedMessage;
    parsedMessage.message = message;
    parsedMessage.messageLength = message.length();
    parsedMessage.errorFlag = 0;
    parsedMessage.trailing = "";
    std::string trimmedMessage = trimFunction(message);
    std::string token;
    std::istringstream iss(trimmedMessage);
    bool tagFlag = false;
    bool tagCmd = true;


    if (trimmedMessage[0] == '@') 
        tagFlag = true;

    while (iss >> token) {
        if (tagFlag) {
            if (token[0] == ':') {
                parsedMessage.command = token.substr(1);
                tagFlag = false;
                tagCmd = false;
                continue;
            }
            continue;
        }

        if (tagCmd) {
            parsedMessage.command = token;
            tagCmd = false;
            continue;
        }

        if (token[0] == ':') {
            parsedMessage.trailing = trimFunction(message.substr(message.find(token)));
            break;
        } else {
            if (isValidParam(token))
                parsedMessage.params.push_back(token);
            else {
                parsedMessage.errorFlag = 1;
                parsedMessage.errorMessage = "Invalid parameter: " + token;
                break;
            }
        }

    }

    return parsedMessage;
}

void printCommand(ircMessage message)
{
    if (message.errorFlag == 1) {
        std::cout << message.errorMessage << std::endl;
        return;
    }

    std::cout << "\nMessage: " << message.message << std::endl;
    std::cout << "Message Length: " << message.messageLength << std::endl;
    std::cout << "Command: " << message.command << std::endl;
    for (size_t i = 0; i < message.params.size(); i++) {
        std::cout << "Param " << i << ": " << message.params[i] << std::endl;
    }
    std::cout << "Trailing: " << message.trailing << std::endl;
}
