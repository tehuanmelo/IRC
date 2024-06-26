#pragma once
#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__
#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <map>
#include <cstring>
/*
*/
class Client
{
	/* VARIABLES*/
	public:
		int 		client_fd;
		int 		pass_status;
		std::string line;
		std::string nickname;   // Nickname chosen by the client
		std::string username;  // Username of the client on the host
		std::string realname;  // Realname of the client on the host
		std::string hostname; // Real name/address of the host
		std::string server;   // Server to which the client is connected
		std::string channel;   // channel to which the client is connected
		std::vector<std::string> toSend;   // channel to which the client is connected
		char 		buffer[1024];
		int 		port;
		// ****** CHANNEL ATRIBUTES ****** //
		std::string currentChannel;
		// ****** CHANNEL MEMBERS****** //

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
	int addChannelInClient(std::string& channel);
	bool registerstatus();
	/*extra*/

};
#endif
