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

struct DataIN  din;
struct DataOUT dout;
timeval LastReceived;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_t rxThread;
pthread_t txThread;
pthread_t bh280Thread;


void *rxLoop(void *threadid)
{	
	Server.rxLoop();
}
void *txLoop(void *threadid)
{	
	Server.txLoop();
}
void *bh280Loop(void *threadid)
{	
	bh280.Loop();
}

int main(int argc, char* argv[])
{
	App.PrintBanner();
	Config.FromArgs(argc, argv);
	Config.PrintConf();
		
	char * rxPnt=Config.use262 ? &din.t262 : &din.t280;
	int rxl=0;
	rxl +=Config.use262? 1+4*4 : 0;
	rxl +=Config.use280? 1+4*4 : 0;
	
	char * txPnt=Config.use262 ? (char *)&dout.m262 : (char *)&dout.m280;
	int txl=2;
	txl +=Config.use262? 4*4*4 : 0;
	txl +=Config.use280? 4*4*4 : 0;
	txl +=Config.usePPS? 4*24*4: 0;
	
	Server.Init(	Config.RXport, Config.TXport,
					rxPnt, txPnt,
					rxl, txl,
					&mutex1,
					&LastReceived,
					Config.ST);//(Config);
/*
	bh280.Initialize(Config.usePPS,
						&din,&dout,
						&mutex1,
						&LastReceived);
	*/					
	pthread_create(&rxThread, 		NULL, rxLoop, NULL);
	pthread_create(&txThread, 		NULL, txLoop, NULL);
	//pthread_create(&bh280Thread, 	NULL, bh280Loop, NULL);

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
		cout<<"cmd> ";
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

