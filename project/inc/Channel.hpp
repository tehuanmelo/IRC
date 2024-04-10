#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "common.hpp"
#include "Reply.hpp"
#include "Client.hpp"
#include "parsing.hpp"


class Channel {
    private:

        std::string             name;
        std::string             topic;
        std::string             password;
		int						userLimit;
        std::map<int, Client*>  users;
        std::map<int, Client*>  operators;
        std::map<char, int>    modes;

    public:

        Channel();
        Channel(std::string& name, Client& user);
        Channel(const Channel& copy);
        Channel& operator=(const Channel& copy);
        ~Channel();

        void                    addUser(Client& user);
        void                    removeUser(Client& user);
        void                    addOperator(Client& user);
        void                    removeOperator(Client& user);
        void                    broadcast(std::string message);

        /////   GETTERS    //////

        std::string             getName();
        std::string             getTopic();
        std::string             getPassword();
        int                     getUserLimit();
        std::map<int, Client*>  getUsers();
        std::map<int, Client*>  getOperators();
        std::map<char, int>     getModes();
        std::string             getListOfUsers();

        /////   SETTERS    //////

        void                    setName(std::string name);
        void                    setTopic(std::string topic, Client& user);
        void                    setPassword(std::string password);
        void                    setUserLimit(int userLimit);
        void                    setMode(char mode, int value);

        /////   CHECKERS    //////
        int                     howManyUsers();
        bool                    isUser(Client& user);
        bool                    isOperator(Client& user);
        bool                    hasTopic();
        bool                    isMode(char mode);

        /////   UTILS   //////

        static void             validate_channels(std::vector<std::string>& params);
};



#endif