#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Reply.hpp"



Channel::Channel() {
    this->name = "Default";
	this->topic = "";
	this->password = "";
	this->userLimit = 0;
	this->modes['i'] = 0;
	this->modes['t'] = 0;
	this->modes['k'] = 0;
	this->modes['l'] = 0;
}

Channel::Channel(std::string& name, Client& user) {
    this->name = name;
	this->topic = "";
	this->password = "asdf";
	this->userLimit = 3;
	this->modes['i'] = 0;
	this->modes['t'] = 1;
	this->modes['k'] = 0;
	this->modes['l'] = 0;
    this->addUser(user);
    this->addOperator(user);
}

Channel::Channel(const Channel& copy) { *this = copy; }

Channel& Channel::operator=(const Channel& copy) {
    this->name = copy.name;
    this->topic = copy.topic;
    this->password = copy.password;
    this->userLimit = copy.userLimit;
    this->modes = copy.modes;
    this->users = copy.users;
    this->operators = copy.operators;
    this->inviteds = copy.inviteds;
    this->it = copy.it;
    return *this;
}

Channel::~Channel() {}

void Channel::addUser(Client& user) {
    this->users[user.client_fd] = &user;
}

void Channel::addOperator(Client& user) {
    this->operators[user.client_fd] = &user;
}

void Channel::addInvited(Client& user) {
    this->inviteds[user.client_fd] = &user;
}

void Channel::removeUser(Client& user) {
    for (it = users.begin(); it != users.end(); it++) {
        if (it->second->client_fd == user.client_fd) {
            this->users.erase(it);
            break;
        }
    }
}

void Channel::removeOperator(Client& user) {
     for (it = operators.begin(); it != operators.end(); it++) {
        if (it->second->client_fd == user.client_fd){
            this->operators.erase(it);
            break;
        }
    }
}

void Channel::removeInvited(Client& user) {
     for (it = inviteds.begin(); it != inviteds.end(); it++) {
        if (it->second->client_fd == user.client_fd){
            this->inviteds.erase(it);
            break;
        }
    }
}

void Channel::removeFromAll(Client& user) {
        this->removeUser(user);
        this->removeOperator(user);
        this->removeInvited(user);
}

void Channel::broadcast(std::string message) {
    std::map<int, Client*>::iterator it = users.begin();
    for (; it != users.end(); it++) 
                it->second->toSend.push_back(message);
}

void Channel::broadcast_others(Client user,std::string message) {
    std::map<int, Client*>::iterator it = users.begin();
    for (; it != users.end(); it++)
    {
        if(it->first != user.client_fd)
            it->second->toSend.push_back(message);
    } 
}
/////   GETTERS     /////

std::string Channel::getName() { return this->name; }

std::string Channel::getTopic() { return this->topic; }

std::string Channel::getPassword() { return this->password; }

int Channel::getUserLimit() { return this->userLimit; }

std::map<int, Client*> Channel::getUsers() { return this->users; }

std::map<int, Client*> Channel::getOperators() { return this->operators; }

std::string Channel::getModes() { 
    std::map<char, int>::iterator it;
    std::string mode = "";
    for (it = this->modes.begin(); it != this->modes.end(); it++) {
        if (it->second) {
            char c = it->first;
            std::string str(1, c);
            mode += str;
        }  
    }
    return mode.empty() ? "" : "+" + mode; 
}

std::map<int, Client*>::iterator Channel::getUser(std::string nickname) {
    for (it = users.begin(); it != users.end(); it++) {
        if (it->second->nickname == nickname)
            break;
    }
    return it;
}

// returns a string with the nicknames of all users in the channel separated by a space
// with prefix @ for operators 
std::string Channel::getListOfUsers() {
    std::string listusers = "";
    std::map<int, Client*>::iterator it = users.begin();
    for (; it != users.end(); it++) {
        if (this->isOperator(*it->second))
            listusers += "@" + it->second->nickname + " ";
        else
            listusers += it->second->nickname + " ";
    }
    return trimFunction(listusers);
}

/////   SETTERS     /////

void Channel::setName(std::string name) { this->name = name; }

void Channel::setPassword(std::string password) { this->password = password; }

void Channel::setUserLimit(int userLimit) { this->userLimit = userLimit; }

void Channel::setMode(char mode, int value) { this->modes[mode] = value; }

bool Channel::isModeSet(char mode) { return this->modes[mode]; }    

void Channel::setTopic(std::string topic) { this->topic = topic; }

/////   CHECKERS    //////

int Channel::howManyUsers() { return this->users.size(); }

bool Channel::isUser(Client& user) {  return this->users.find(user.client_fd) != this->users.end(); }

bool Channel::isUser(std::string& user) {
    for (it = this->users.begin(); it != this->users.end(); it++) {
        if (it->second->nickname == user)
            break;
    }
    return it != this->users.end();
}

bool Channel::isOperator(Client& user) { return this->operators.find(user.client_fd) != this->operators.end(); }

bool Channel::isInvited(Client& user) { return this->inviteds.find(user.client_fd) != this->inviteds.end(); }

bool Channel::hasTopic() { return this->topic != ""; }

bool Channel::isMode(char mode) { return this->modes[mode]; }


/////   UTILS   /////







