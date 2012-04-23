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