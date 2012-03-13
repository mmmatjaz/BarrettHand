#include "server.h"
#include <sstream>
#include <pthread.h>

#define BUFFSIZE 255

extern pthread_mutex_t mutex1;
/*	Default constructor. Nothing to see here, cause the class must be initialized
 * 	before main() (so its global), but the necessary parameters are not
 * 	available before main() -> see ServerSetup */
void Server::Init( 	int RXport, int TXport, 
					char * rxPnt, char *txPnt,
					int rxLength, int txLength,
					pthread_mutex_t * mutex,
					timeval * LastReceived,
					double ST)
{
		portTX=TXport;		portRX=RXport;
		rxpnt=rxPnt;		txpnt=txPnt;
		rxl=rxLength;		txl=txLength;
		Mutex=mutex;
		timeStamp=LastReceived;
		SampleTime=ST;
		int nTimeout = 100;
		
		sockRX=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		setsockopt(sockRX, SOL_SOCKET, SO_REUSEADDR, 0, 0);  
		if (sockRX < 0) Die("Opening RX socket failed");
		//if (sockTX < 0) Die("Opening TX socket failed");
		int flags;

		length = sizeof(server);
		bzero(&server,length);

		server.sin_family=AF_INET;
		server.sin_addr.s_addr=INADDR_ANY;
		server.sin_port=htons(portRX);
		from.sin_family=AF_INET;
		from.sin_port = htons(portTX);
	  
		if (bind(sockRX,(struct sockaddr *)&server,length)<0)
		   Die("Binding RX socket failed");
		fromlen = sizeof(struct sockaddr_in);
		
		shouldRun=true;
		gettimeofday(&begin, NULL);
		
		bool nonblocking=false;
		
		if (nonblocking)
		{
			int flags = fcntl(sockRX, F_GETFL);
			if (fcntl(sockRX, F_SETFL, O_NONBLOCK) < 0)
				perror("set fcntl flags failed.\n");
		}
		
}

Server::~Server()
{
}
/* Receiving-thread routine*/
void Server::rxLoop()
{
	char message[100];
	char rxBuff[rxl];
	int rxBufflen=rxl;
	int received;
	
	while (shouldRun)
	{
		memset(&rxBuff[0], 0, sizeof(rxBuff));
		rxBuff[0]='\0';
		//cout<<"cakam"<<endl;
		received = recvfrom(sockRX,rxBuff,rxl,0,(struct sockaddr *)&from,&fromlen);
		from.sin_port = htons(portTX);
		if (received < 0) Die("rx");
		//cout<<"dobil"<<endl;
		
	pthread_mutex_lock( Mutex );
	gettimeofday(timeStamp, NULL);
	memcpy(rxpnt,rxBuff,rxl);
	pthread_mutex_unlock( Mutex );
		
		gettimeofday(&current, NULL);
		//cout<<DiffClock(&current,&begin)<<endl;
		
		if (!haveIP)	
			haveIP=true;		
	}	
}
/* Receiving-thread routine*/
void Server::Pong()
{
	char message[100];
	char rxBuff[rxl];
	char txBuff[txl];
	int rxBufflen=rxl;
	int received;
	int n;
	double T;
	while (shouldRun)
	{
		memset(&rxBuff[0], 0, sizeof(rxBuff));
		rxBuff[0]='\0';
		//cout<<"cakam"<<endl;
		received = recvfrom(sockRX,rxBuff,rxl,0,(struct sockaddr *)&from,&fromlen);
		//from.sin_port = htons(portTX);
		if (received < 0) Die("rx");
		//cout<<"dobil"<<endl;
		
	pthread_mutex_lock( Mutex );
	gettimeofday(timeStamp, NULL);
	memcpy(rxpnt,rxBuff,rxl);
	memcpy(txBuff,txpnt,txl);
	pthread_mutex_unlock( Mutex );
		
		gettimeofday(&current, NULL);
		T=DiffClock(&current,&begin);
		memcpy(txBuff,&T,sizeof(double));
		n = sendto(sockRX,txBuff,txl,
			0,(struct sockaddr *)&from,fromlen);
		if (n  < 0) Die("tx");	
			
	}	
}
/*	Sending-thread routine*/
void Server::txLoop()
{
	struct timeval time2, time1, time0;
	double dt;
	gettimeofday(&time0, NULL);
	int n;
	
	char message[100];
	char txBuff[txl];
	
	
	while (shouldRun)
	{		
		while (!haveIP && shouldRun){}
		dt=SampleTime/10;
		gettimeofday(&time1, NULL);
		
		while(dt<SampleTime || !haveIP)
		{
			gettimeofday(&time2, NULL);
			dt=DiffClock(&time2,&time1);
			usleep(SampleTime*1000000/10);
		}	
		
		pthread_mutex_lock( Mutex );
		memcpy(txBuff,txpnt,txl);
		pthread_mutex_unlock( Mutex );
		
		n = sendto(sockTX,txBuff,txl,
			0,(struct sockaddr *)&from,fromlen);
		if (n  < 0) Die("tx");	
	}
	cout<<"Server TX compl33t\n";
}

double Server::DiffClock(timeval* currentTime, timeval* startTime)
{
	double t2=(double)currentTime->tv_sec+(double)currentTime->tv_usec/1000000.0;
	double t1=(double)startTime->tv_sec+(double)startTime->tv_usec/1000000.0;
	if (t2-t1 > 0.0)
	{
		return t2-t1;
	}
	else
	{
		return 1.0;
	}
}

void Server::Stop()
{
	shouldRun=false;
}

void Server::Die(char *mess)
{
	perror(mess);
}
