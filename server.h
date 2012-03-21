
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/time.h>// Needed for ms time.
#include <time.h>// Needed for ms time.
#include <math.h>
#include <fcntl.h>
#include <string>
#include <iostream>

#include "app.h"

using namespace std;


struct Setup
{
	int RXport;
	int TXport;
	char * rxBuff;
	//int rxBuffLen;
	char * txBuff;
	//int rxBuffLen;
	float SampleTime;
	
	timeval * LastReceived;
	int RXsock;
	int TXsock;
	
	struct sockaddr_in RXaddr;
	unsigned int RXaddrLen;
	struct sockaddr_in TXaddr;
	unsigned int TXaddrLen;
	
	
	
};


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
	//void *Pong();
	static void* RunServer(void *ptr){return (Server *) ptr->Pong(); };
	void rxLoop();
	void txLoop();
	void Stop();
	/*
	void *run();
	static void* run_server(void *ptr){return (serverremote *) ptr->run(); };
	*/
	/*
	if(pthread_create(&thread_server, &tattr_server, &ServerRemote::run_server, (void *) this) != 0)
	void* run();  //kle not je zanka: void *ServerRemote::run()
	static void* run_server(void *ptr) { return ((ServerRemote *) ptr)->run(); }
	*/
};

