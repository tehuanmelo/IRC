#pragma once

#include "common.hpp"
#include "utils.hpp"
#include "Reply.hpp"
#include "Client.hpp"


class Channel {
    private:

        std::string                         name;
        std::string                         topic;
        std::string                         password;
        // std::time_t                         creationTime;
		int						            userLimit;
        std::map<char, int>                 modes;
        std::map<int, Client*>              users;
        std::map<int, Client*>              operators;
        std::map<int, Client*>              inviteds;
        std::map<int, Client*>::iterator    it;

    public:

        Channel();
        Channel(std::string& name, Client& user);
        Channel(const Channel& copy);
        Channel& operator=(const Channel& copy);
        ~Channel();

        void                                addUser(Client& user);
        void                                removeUser(Client& user);
        void                                addOperator(Client& user);
        void                                removeOperator(Client& user);
        void                                broadcast(std::string message);
        void                                broadcast_others(Client user,std::string message);
        void                                addInvited(Client& user);
        void                                removeInvited(Client& user);
        void                                removeFromAll(Client& user);

        /////   GETTERS    //////

        int                                 getUserLimit();
        std::string                         getName();
        std::string                         getTopic();
        std::string                         getPassword();
        std::string                         getListOfUsers();
        std::map<int, Client*>              getUsers();
        std::map<int, Client*>              getOperators();
        std::string                         getModes();
        std::map<int, Client*>::iterator    getUser(std::string);

        /////   SETTERS    //////

        void                                setName(std::string name);
        void                                setTopic(std::string topic);
        void                                setPassword(std::string password);
        void                                setUserLimit(int userLimit);
        void                                setMode(char mode, int value);

        /////   CHECKERS    //////

        int                                 howManyUsers();
        bool                                isUser(Client& user);
        bool                                isUser(std::string& user);
        bool                                isOperator(Client& user);
        bool                                isInvited(Client& user);
        bool                                hasTopic();
        bool                                isMode(char mode);
        bool                                isModeSet(char mode);


};


