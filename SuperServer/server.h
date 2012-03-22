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

class SuperServer
{
private:
	pthread_t threadServer;
//socket
	int length;
	int sockRX, sockTX;
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	int Port;
//data
	pthread_mutex_t * Mutex;
	Command * Comm;
	Reply *	Rep;
	timeval * timeStamp;
//general
	bool haveIP;
	bool shouldRun;
	bool isRunning;
	timeval begin, current;
//methods
	void ReceiveCommands();
	void WaitForHands();
	void SendReply();
	void error(const char *msg);
	void Die(char *mess);
	double DiffClock(timeval* currentTime, timeval* startTime);
public:
	SuperServer(int port,
				Command * Comm_, Reply * Rep_,
				pthread_mutex_t * mutex,
				timeval * LastReceived);
	
	~SuperServer();
	void *Pong();
	void Start();
	static void* RunServer(void *ptr){return ((SuperServer *) ptr)->Pong(); };
	void Stop();
	
};

#endif