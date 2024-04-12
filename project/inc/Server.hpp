#pragma once
#ifndef __SERVER_HPP__
#define __SERVER_HPP__
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <stdlib.h>
#include <algorithm>
#include <signal.h>
#include <vector>
#include <map>
#include <fcntl.h>
#include <cstring>
#include "Client.hpp"
#include "parsing.hpp"
#include "Channel.hpp"
#include "common.hpp"
#include "Reply.hpp"

#include <netdb.h>
/*
*/
// static int g_signal;
class Server
{
	/* VARIABLES*/
	private:
	/*   server socket variables  */
		int server;
		sockaddr_in service;
		int port;
		std::string ports;
		std::string server_name;
		std::vector <struct pollfd> fd_poll;
		int guestuser;
	/*   server socket variables  */
	/*   server authentication  */
		std::string pass;
	/*   server authentication  */

	/*   server info  */
		std::string creation_date;
		int number_of_clients;
		std::vector <Client*> client;
		std::vector <std::string> nicknames;
	/*   server info  */

	/*   future extras  */
		/*Capability Negotiation Settings for IRSSI SERVER SIDE*/
		// bool multi_prefix;
		// bool extended_join;
		// bool away_notify;
		// bool chghost;
		// bool account_notify;
		// bool server_time;
		// std::string sasl;
		/*complex settings*/
		// bool tls;
		// bool account_tag;
		// bool cap_notify;
		// bool echo_message;
		/*complex settings*/
		/*Capability Negotiation Settings*/
	/*   future extras  */

    /* VARIABLES*/
	public:
	/*orth form*/
    	Server ();
		Server (std::string port,std::string pass);

    	Server (const Server &a);
    	~Server ();
		Server& operator=(const Server &a);
	/*orth Server*/
	/*exception*/
		void			sighandle(int sig);
		int				serverInit();
		int				runServer();
		int				serverLoop();
		int				connectionEvent();
		// void				register_user(ircMessage msg, Client * user);
		int				register_user(ircMessage msg, Client * user);
		void 			adduser(Client * user, ircMessage msg);
		void			nick(Client * user, std::string str);
		// std::string int_tostring(int guest);
		
		int				Recv_end(int fd, std::string & line);
		void			commandPath(ircMessage msg, Client * user);
		void 			callMotd(Client * user);
		int 			MOTD(Client * user);
		int 			definedmessage(int fd,std::string str);
		std::string		msg(std::string source, std::string command, std::string param, std::string text);
		// std::string 	cap_ls();
		// std::string 	cap_ack(ircMessage cap_list);
		void 	cap_ls(Client *user);
		void 	cap_ack(Client *user);
		std::string 	date_now();


		// ********** CHANNEL ********** //


		std::map<std::string, Channel>	channels;

		bool 							isUser(Client& user);
		bool 							isUserNick(std::string nickname);

		int 							addChannelInServer( Channel& channel);
		bool 							hasChannelInServer(std::string channelName);

		// ********** COMMANDS ********** //

		void 		joinCommand(ircMessage msg,  Client& user);
		void 		topicCommand(ircMessage msg, Client& user);
		void 		kickCommand(ircMessage msg, Client& user);
		void 		inviteCommand(ircMessage msg, Client& user);
		void 		privmsgCommand(ircMessage msg, Client& user);
	/*exception*/
	/*getters and setters*/
	/*getters and setters*/
	/*extra*/
	/*extra*/
};



#endif
