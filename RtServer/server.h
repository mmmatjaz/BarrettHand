/*
 This file is part of "IJS BarrettHand Utils".

    IJS BarrettHand Utils is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3.

    IJS BarrettHand Utils is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with IJS BarrettHand Utils.  If not, see <http://www.gnu.org/licenses/>.
	
	author: Matjaž Ogrinc
			matjaz.ogrinc42@gmail.com
			https://github.com/mmmatjaz
			
			IJS 2011-2012
			abr.ijs.si
*/	


#ifndef SERVER_H
#define SERVER_H

#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>// Needed for ms time.
#endif

#ifdef _WIN32 || defined _WIN64
#include <WinSock2.h>
typedef int socklen_t;
#endif

#include "app.h"

using namespace std;

class Server
{
private:
//socket
	int length;
	int sockRX, sockTX;
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	int portTX;
	int portRX;

//data
	double SampleTime;	
	pthread_mutex_t * Mutex;
	char *rxpnt;
	char *txpnt;
	int rxl;
	int txl;
	timeval * timeStamp;
//general
	bool haveIP;
	bool shouldRun;
	timeval begin, current;
//methods
	void error(const char *msg);
	void Die(char *mess);
	double DiffClock(timeval* currentTime, timeval* startTime);
public:
	void Init( 	int RXport, int TXport, 
				char * rxPnt, char *txPnt,
				int rxLength, int txLength,
				pthread_mutex_t * mutex,
				timeval * LastReceived,
				double ST);
	~Server();
	void Pong();
	void Stop();
};

#endif