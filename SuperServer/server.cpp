#include "server.h"

#define SIGNATURE "Server:"

void SuperServer::Init( 	int port,
							Command * Comm_, Reply * Rep_,
							pthread_mutex_t * mutex,
							timeval * LastReceived)
{
		Port=port;	
		Comm=Comm_;			Rep=Rep_;
		Mutex=mutex;
		timeStamp=LastReceived;
		int nTimeout = 100;
		
	#ifdef _WIN32 || defined _WIN64
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2,2), &wsaData);
	#endif

		sockRX=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		setsockopt(sockRX, SOL_SOCKET, SO_REUSEADDR, 0, 0);

		//setsockopt(sockRX, SOL_SOCKET, SO_RCVTIMEO, (const char*)&nTimeout, sizeof(int)); 
		//setsockopt(sockRX, SOL_SOCKET, SO_SNDTIMEO, (const char*)&nTimeout, sizeof(int));

		if (sockRX < 0) Die("Opening RX socket failed");
		//if (sockTX < 0) Die("Opening TX socket failed");
		int flags;

		length = sizeof(server);
		memset(&server,0,length);

		server.sin_family=AF_INET;
		server.sin_addr.s_addr=INADDR_ANY;
		server.sin_port=htons(Port);
		from.sin_family=AF_INET;
		from.sin_port = htons(Port);
	  
		if (bind(sockRX,(struct sockaddr *)&server,length)<0)
		   Die("Binding RX socket failed");
		fromlen = sizeof(struct sockaddr_in);
		
		shouldRun=true;
		gettimeofday(&begin, NULL);
		
		cout << "Server initialized"<<endl;
}


void SuperServer::ReceiveCommands()
{
	int rxl=10*8;
	char * rxBuff= new char[rxl];
	memset(rxBuff, 0, rxl);
	int received = recvfrom(sockRX,rxBuff,rxl,0,(struct sockaddr *)&from,&fromlen);
	if (received < 0) Die("rx");

	int afnaFound=0;
	int ci1=0;
	int ci2=0;
pthread_mutex_lock( Mutex );
	for (int i=0; i<received; i++)
	{
	   if (afnaFound==1 && rxBuff[i]!=64)
	   {
		   Comm->command262[ci1]=rxBuff[i];
		   ci1++;
	   }
	   if (afnaFound==2 && rxBuff[i]!=64)
	   {
		   Comm->command280[ci2]=rxBuff[i];
		   ci2++;
	   }
	   if (rxBuff[i]==64)
		   afnaFound++;
	}
	Comm->command262[ci1]=0;
	Comm->command280[ci2]=0;
	Rep->executed262=false;
	Rep->executed280=false;
pthread_mutex_unlock( Mutex );
	cout<<SIGNATURE<<" Received "<< received << " bytes from " << &from.sin_addr << endl;
}

void SuperServer::WaitForHands()
{
	bool wait=true;
	while (wait)
	{
		usleep(100000);
		pthread_mutex_lock( Mutex );
		wait=!(Rep->executed262) && !(Rep->executed280);
		pthread_mutex_unlock( Mutex );
	}
}

void SuperServer::SendReply()
{
	int txl=10*8;
	char txBuff[128]="";

	pthread_mutex_lock( Mutex );
	strcat(txBuff,"@");
	strcat(txBuff,Rep->reply262);
	strcat(txBuff,"@");
	strcat(txBuff,Rep->reply280);
	pthread_mutex_unlock( Mutex );
	int TXbufferSize=sizeof(txBuff);
	int n = sendto(sockRX,txBuff,TXbufferSize,
			0,(struct sockaddr *)&from,fromlen);
		if (n  < 0) Die("tx");	
	cout<<SIGNATURE<<" Replied "<<endl;
	
}
/* Receiving-thread routine*/
threadMethod SuperServer::Pong()
{
	cout << "Server thread started" <<endl;
	isRunning=true;
	while (shouldRun)
	{		
		ReceiveCommands();
		WaitForHands();
		SendReply();			
	}	
	isRunning=false;
	cout << "Server thread ended" <<endl;
}
/*	Sending-thread routine*/
double SuperServer::DiffClock(timeval* currentTime, timeval* startTime)
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

void SuperServer::Stop()
{
	cout<<"Server thread killed"<<endl;
}

void SuperServer::Die(char *mess)
{
	perror(mess);
	#ifdef _WIN32 || defined _WIN64
		WSACleanup();
	#endif
}
