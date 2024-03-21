#pragma once
#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__
#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <cstring>
/*
*/
class Client
{
	/* VARIABLES*/
	public:
		int client_fd;
		int regi_status;
		std::string hostname; // Real name/address of the host
		std::string ip; // Real name/address of the host
		std::string username;  // Username of the client on the host
		std::string realname;  // Realname of the client on the host
		std::string nickname;   // Nickname chosen by the client
		std::string server;   // Server to which the client is connected
		std::string channel;   // channel to which the client is connected
<<<<<<< HEAD
=======
		// struct sockaddr_in my_addr;
		// socklen_t addrlen = sizeof(my_addr);
>>>>>>> dfb13bf7c1f75cfed76bf1c56784cc386c04efe9

		char buffer[1024];
		
		int port;
		/*Capability Negotiation Settings for IRSSI CLIENT SIDE*/
		bool multi_prefix;
		bool extended_join;
		bool away_notify;
		bool chghost;
		bool account_notify;
		bool server_time;
		/*Capability Negotiation Settings*/

    /* VARIABLES*/
	// public:
	/*orth form*/
    	Client ();
    	Client (const Client &a);
    	~Client ();
		Client& operator=(const Client &a);
	/*orth Client*/
	/*exception*/

	
	/*exception*/
	/*getters and setters*/
	/*getters and setters*/
	/*extra*/
	/*extra*/
};
#endif