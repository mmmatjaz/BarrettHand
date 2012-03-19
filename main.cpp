#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <sys/time.h>// Needed for ms time.
#include <time.h>// Needed for ms time.
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>
#include <fcntl.h>
#include <iostream>
#include "app.h"
#include "server.h"
#include "bh280.h"

using namespace std;

App		App;
Config	Config;
Server 	Server;
BH280	bh280;

 Controls  ConsG;
 Measurements MeasG;
timeval LastReceived;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_t rxThread;
pthread_t txThread;
pthread_t bh280Thread;


void *rxLoop(void *threadid)
{	
	Server.Pong();
}
void *bh280Loop(void *threadid)
{	
	//bh280.Loop();
	bh280.LoopOfflineTorque();
}

int main(int argc, char* argv[])
{
	App.PrintBanner();
	Config.FromArgs(argc, argv);
	Config.PrintConf();
	
	bzero(&ConsG,sizeof(ConsG)); bzero(&MeasG,sizeof(MeasG));	
	
	int rxl=sizeof(ConsG);
	int txl=sizeof(MeasG);
	if (!Config.usePPS) txl -=sizeof(MeasG.hdata280.pps);
	
	printf("rxl %i txl %i \n",rxl,txl);
	Server.Init(	Config.RXport, Config.TXport,
					(char *) &ConsG, (char *) &MeasG,
					rxl,txl,
					&mutex1, &LastReceived,
					Config.ST);//(Config);
					
	bh280.Initialize(Config.usePPS,
						&ConsG.con280,&MeasG.hdata280,
						&mutex1,
						&LastReceived);	
	
	pthread_create(&rxThread, &tattr_server, &Server::RunServer, (void *) this)	
	pthread_create(&rxThread, 		NULL, rxLoop, NULL);
	pthread_create(&bh280Thread, 	NULL, bh280Loop, NULL);

	
	string input;
	string tmp;
	int offset;
	//cout<<"\ncmd> ";

	while (true)
	{
		
		getline(cin,input);
		offset=input.find("q");
		if (offset>-1 && input.size()==1)
		{
		  cout<<"quit\n"<<endl;
		  break;
		}
		
		cout<<endl;
	
	}
	
	Server.Stop();
	bh280.Stop();
	Server;
	bh280;
	 pthread_join( rxThread, NULL);
	  pthread_join( txThread, NULL);
	   //pthread_join( bh280Thread, NULL);
	printf("\n");
	return 0;
}

