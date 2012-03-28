
#include "server.h"
#include "bh280.h"
#include "app.h"
#include "graphics.h"

using namespace std;

App		App;
Config	Config;
Server 	Server;
BH280	bh280;

Controls  ConsG;
Measurements MeasG;
timeval LastReceived;

pthread_mutex_t mutex1;
pthread_t rxThread;
pthread_t txThread;
pthread_t bh280Thread;


threadMethod rxLoop(void *threadid)
{	
	Server.Pong();
}
threadMethod bh280Loop(void *threadid)
{	
	//bh280.Loop();
	bh280.LoopOffline();
}

int main(int argc, char* argv[])
{
	
	App.PrintBanner();
	Config.FromArgs(argc, argv);
	Config.PrintConf();

	pthread_mutex_init(&mutex1, NULL);
	memset(&ConsG,0,sizeof(ConsG)); memset(&MeasG,0,sizeof(MeasG));	
	
	int rxl=sizeof(ConsG);
	int txl=sizeof(MeasG);
	if (!Config.usePPS) txl -=sizeof(MeasG.hdata280.pps);
	
	Server.Init(	Config.RXport, Config.TXport,
					(char *) &ConsG, (char *) &MeasG,
					rxl,txl,
					&mutex1, &LastReceived,
					Config.ST);//(Config);
					
	bh280.Initialize(Config.usePPS,
						&ConsG.con280,&MeasG.hdata280,
						&mutex1,
						&LastReceived);	
	
	//pthread_create(&rxThread, &tattr_server, &Server::RunServer, (void *) this)	
	//pthread_create(&rxThread, 		NULL, rxLoop, NULL);
	pthread_create(&bh280Thread, 	NULL, bh280Loop, NULL);
	initGlut(argc, argv, &MeasG.hdata280, &mutex1);
	
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
	
	 pthread_join( rxThread, NULL);
	  pthread_join( txThread, NULL);
	   //pthread_join( bh280Thread, NULL);
	printf("\n");
	return 0;
}

