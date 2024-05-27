#include "../inc/Server.hpp"

// 21:24 Syntax:
// 21:24 
// 21:24 MODE <your nick>|<chanName> [<mode> [<mode parameters>]]
// 21:24 
// 21:24 Parameters:
// 21:24 
// 21:24     The target nickname or chanName and the modes with their parameters to set or
// 21:24     remove.
// 21:24 
// 21:24     If the target nickname or chanName is omitted, the active nickname or chanName
// 21:24     will be used.
// 21:24 
// 21:24 Description:
// 21:24 
// 21:24     Modifies the user or chanName modes for which you are privileged to modify.
// 21:24 
// 21:24     You can specify multiple modes in one command and prepend them by using the
// 21:24     '+' sign to set or '-' sign to unset; modes that require a parameter will be
// 21:24     retrieved from the argument list.
// 21:24 
// 21:24     Some common chanName modes are:

// 21:24         i:               When enabled, users need to be invited into the chanName
// 21:24                          or have a matching entry in the invite list.
// 21:24         k <key>:         Adds or removes a chanName key, aka a password; users
// 21:24                          will not be able to join the chanName without providing
// 21:24                          the key.
// 21:24         l <count>:       Adds, removes or updates the maximum amount of users
// 21:24         t:               When enabled, users who are not opped or voices cannot
// 21:24                          modify the chanName topic.
// 21:24         o <nickname>:    Adds or removes the operator status from a nickname..
// 21:24 
// 21:24     /MODE #irssi +o mike
// 21:24     /MODE #irssi -o+iI mike mike!*@*.irssi.org
// 21:24     /MODE +b mike!*@*.irssi.org
// 21:24     /MODE -o mike
// 21:24     /MODE +impsnkl secret_hideout 100
// 21:24     /MODE mike +iw

struct Mode {
    int signal;
    char mode;
    std::string parameter;
};

struct ModeMessage {
    std::string channel;
    std::string modestring;
    std::vector<std::string> modeParams;
};

std::vector<Mode> getModes(Channel& channel, ModeMessage& input) {
    
    std::vector<Mode> modes; // map to store the values the key is the mode 'x' the value is a pair<int,str> the int is the value to be set on/off the string is the mode parameter
    int flag; // variable to store the value on/off
    int paramIndex = 0;
    Mode mod;

    

    for (size_t i = 0; i < input.modestring.length(); i++) {
            char c = input.modestring[i];
            if (c == '+')
                flag = 1;
            else if (c == '-')
                flag = 0;
            else if (c == 'k' || c == 'l') { // these two are special modes (key and limit) that require parameters
                if (flag) { // if the mode is to set on a parameter is required
                    if (!input.modeParams.empty()) {
                        if (c == 'l') {
                            int newLimit = std::atoi(input.modeParams[paramIndex].c_str());
                            if (newLimit == 0 && input.modeParams[paramIndex] != "0") {
                                paramIndex++;
                                continue;
                            }
                        }
                        mod.mode = c;
                        mod.signal = flag;
                        mod.parameter = input.modeParams[paramIndex++];
                        modes.push_back(mod);
                    }
                } 
                else {
                    if (channel.isModeSet(c) != flag) {
                        mod.mode = c;
                        mod.signal = flag;
                        mod.parameter = "";
                        modes.push_back(mod);
                    }
                }
            } 
            else if (c == 'o') { // the mode operator requires parameter either for on and off
                if (!input.modeParams.empty()) { // if there is parameter on the parameters vector
                        mod.mode = c;
                        mod.signal = flag;
                        mod.parameter = input.modeParams[paramIndex++];
                        modes.push_back(mod);
                }
            }
            else if (c == 'i' || c == 't') {
                if (channel.isModeSet(c) != flag) {
                        mod.mode = c;
                        mod.signal = flag;
                        mod.parameter = "";
                        modes.push_back(mod);
                    }
            }
        
        }

        return modes;
}

std::string getModestringSeted(std::vector<Mode> modes) {
    std::string plusModes = "";
    std::string minusModes = "";
    std::string modesParams = "";
    for (size_t i = 0; i < modes.size(); i++) {
        std::string str(1, modes[i].mode);
        if (modes[i].signal == 1) {
            plusModes += str;
            modesParams += modes[i].parameter+" ";
        } 
        else {
            minusModes += str;
            if (str[0] == 'o')
                modesParams += modes[i].parameter+" ";

        }
    }
    std::string finalstring = "";
    minusModes.empty() ? "" : finalstring += "-"+minusModes;
    plusModes.empty() ? "" : finalstring += "+"+plusModes;
    
    return finalstring +" "+trimFunction(modesParams);
}

ModeMessage parseModeMessage(ircMessage& msg) {
    ModeMessage newModes;

    for (size_t i = 0; i < msg.params.size(); i++) {
        if (i == 0)
            newModes.channel = msg.params[i];
        else if (i == 1)
            newModes.modestring = msg.params[i];
        else            
            newModes.modeParams.push_back(msg.params[i]);
    }

    if (!msg.trailing.empty()) {
        std::vector<std::string> tokens = split(msg.trailing, ' ');
        for (size_t i = 0; i < tokens.size(); i++) {
            if (i == 0) { // if is the first word from the trailing
                newModes.modeParams.push_back(tokens[i].substr(1));  // remove the colom and store into the mode params
                continue;
            }
            newModes.modeParams.push_back(tokens[i]);
        }
    }

    return newModes;
}

bool validateModeString(ModeMessage input) {
    size_t counter = 0;
    int flag;

    for (size_t i = 0; i < input.modestring.length(); i++) {
        if (input.modestring[i] == '+')
            flag = 1;
        else if (input.modestring[i] == '-')
            flag = 0;
        
        if (((input.modestring[i] == 'k' || input.modestring[i] == 'l') && flag == 1) || input.modestring[i] == 'o') {
            std::cout << "this is the character: " << input.modestring[i] << std::endl;
            counter++;
        }
    }
    std::cout << "Number of modes: " << counter << std::endl;
    std::cout << "Number of params: " << input.modeParams.size() << std::endl;
    std::cout << (counter == input.modeParams.size()) << std::endl;
    return counter == input.modeParams.size();
}

void Server::modeCommand(ircMessage& msg, Client& user) 
{
    // **************************************************** //
    //  this is the first error handling block
    //  checks if modesInput.channel wass passed and if
    //  it is valid
    // **************************************************** //
    std::string reply;

    if (msg.params.size() < 1) {
        user.toSend.push_back(ERR_NEEDMOREPARAMS(user.nickname, "MODE"));
        return;
    }
    else if (msg.params[0][0] != '#')
    {
        std::string target = msg.params[0];
        if(this->isUserNick(target))
        {
            if(msg.params.size() > 1)
                reply = ERR_UMODEUNKNOWNFLAG(user.nickname);
            else
                reply =  RPL_CHANNELMODEIS(user.nickname,"", "");
        }
        else
            reply = ERR_USERSDONTMATCH(user.nickname);
        user.toSend.push_back(reply);
        return;
    }
    // **************************************************** //


    ModeMessage modesInput = parseModeMessage(msg);
    this->chan_it = this->getChannelIt(modesInput.channel);
    
    
    if (this->chan_it == this->channels.end()) {
        reply = ERR_NOSUCHCHANNEL(user.nickname, modesInput.channel);
    } else if (!validateModeString(modesInput)) {
        reply = ERR_NEEDMOREPARAMS(user.nickname, "MODE");
    }
    else if (modesInput.modestring.find_first_not_of("+-kloit") != std::string::npos) {
        std::size_t charPosition = modesInput.modestring.find_first_not_of("+-kloit");
        char c = modesInput.modestring[charPosition];
        std::string charString(1, c);
        reply = ERR_UNKNOWNMODE(user.nickname, charString);
    }
    else if (msg.params.size() == 1 && this->chan_it->second.isUser(user)) {
        reply = RPL_CHANNELMODEIS(user.nickname, modesInput.channel, this->chan_it->second.getModes());
    }
    else if (msg.params.size() > 1 && !chan_it->second.isOperator(user)) {
        reply = ERR_CHANOPRIVSNEEDED(user.nickname, modesInput.channel);
    }
    else { // if no error detected execute modes

        std::vector<Mode> modes;
        if (this->chan_it != this->channels.end()) {
            modes = getModes(this->chan_it->second, modesInput);

            for (size_t i = 0; i < modes.size(); ++i) {
                if (modes[i].mode == 'l' && modes[i].signal) {
                        this->chan_it->second.setUserLimit(std::atoi(modes[i].parameter.c_str()));
                } 
                else if (modes[i].mode == 'k' && modes[i].signal) {
                    this->chan_it->second.setPassword(modes[i].parameter);
                } 
                else if (modes[i].mode == 'o') {
                    Client* client = this->getUser(modes[i].parameter);
                        if (client != NULL) {
                            if (client && this->chan_it->second.isUser(*client)) {
                                if (modes[i].signal == 1) {
                                    if (!this->chan_it->second.isOperator(*client)) {
                                        this->chan_it->second.addOperator(*client);
                                        std::cout << "inside add operator mode client name " << client->nickname << std::endl;
                                    }
                                }
                                else if (modes[i].signal == 0) {
                                    if (this->chan_it->second.isOperator(*client)) {
                                        this->chan_it->second.removeOperator(*client);
                                    }
                                }
                            } 
                        }
                    continue;
                }
                this->chan_it->second.setMode(modes[i].mode, modes[i].signal);      // /mode +ti
            }
           

        std::string reply = MODE_CHANNELCHANGEMODE(user_id(user.nickname, user.username), modesInput.channel, getModestringSeted(modes));
        this->channels[modesInput.channel].broadcast(reply);
        return;

        }


       

    }

    user.toSend.push_back(reply);

}
   