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

struct Controls  din;
struct Measurements dout;
timeval LastReceived;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_t rxThread;
pthread_t txThread;
pthread_t bh280Thread;


void *rxLoop(void *threadid)
{	
	Server.Pong();
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
	
	bzero(&din,sizeof(din)); bzero(&dout,sizeof(dout));	
	/*
	char * rxPnt=Config.use262 ? (char *)&din.t262 : (char *)&din.t280;
	int rxl=0;
	if (Config.use262) rxl +=sizeof(din)/2;
	if (Config.use280) rxl +=sizeof(din)/2;
	char * txPnt=Config.use262 ? (char *)&dout.m262 : (char *)&dout.m280;
	rxPnt=(char *)&din;
	txPnt=(char *)&dout;
	rxl=sizeof(din);
	txl=2*sizeof(dout.m262);
	*/
	char * rxPnt=(char *) &din;
	int rxl =sizeof(din);
	char * txPnt=(char *) &dout;
	int txl =2*4*4*8;
	if (Config.usePPS) txl +=4*24*4;
	
	
	if (Config.usePPS) txl +=sizeof(dout.pps);
	
	printf("%i %i \n",sizeof(din),sizeof(dout));
	Server.Init(	Config.RXport, Config.TXport,
					rxPnt, txPnt,
					sizeof(din),sizeof(dout),//rxl, txl,
					&mutex1,
					&LastReceived,
					Config.ST);//(Config);
	bh280.Initialize(Config.usePPS,
						&din,&dout,
						&mutex1,
						&LastReceived);	
						
	pthread_create(&rxThread, 		NULL, rxLoop, NULL);
	//pthread_create(&txThread, 		NULL, txLoop, NULL);
	pthread_create(&bh280Thread, 	NULL, bh280Loop, NULL);

	
	struct Controls * test=&din;
	string input;
	string tmp;
	int offset;
	//cout<<"\ncmd> ";
	while (true)
	{
		getline(cin,input);
		printf("mode @%i %f = %f : %f \n", &din.t280, din.t280,test->t280,din.c280[0]);
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

