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
#define MODEL262	1
#define MODEL280	2

#include "server.h"
#include "bh280.h"
#include "bh262.h"
//#include "bhRT.h"
#include "app.h"
#include "graphics.h"

using namespace std;

App		App;
Config	Config;
Server 	Server;
BH280	bh280;
BH262	bh262;

Controls  ConsG;
Measurements MeasG;
timeval LastReceived;

pthread_mutex_t mutex1;
pthread_t serverThread;
//pthread_t txThread;
pthread_t bh280Thread;
pthread_t bh262Thread;


threadMethod rxLoop(void *threadid)
{	
	Server.Pong();
}

threadMethod bh280Loop(void *threadid)
{	
	bh280.Loop();
}

threadMethod bh262Loop(void *threadid)
{	
	bh262.Loop();
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
	//if (!Config.usePPS) txl -=sizeof(MeasG.hdata280.pps);
	
	Server.Init(	Config.port, Config.port,
					(char *) &ConsG, (char *) &MeasG,
					rxl,txl,
					&mutex1, &LastReceived,
					Config.ST);//(Config);
	pthread_create(&serverThread, 	NULL, rxLoop, NULL);

	usleep(500000);
	if (Config.use280){
		bh280.Initialize(Config.usePPS,
					&ConsG.con280,&MeasG.hdata280,
					&mutex1,
					&LastReceived);	
		pthread_create(&bh280Thread, 	NULL, bh280Loop, NULL);
	}
	if (Config.use262){
		bh262.Initialize(Config.serialPort,
					&ConsG.con262,&MeasG.hdata262,
					&mutex1,
					&LastReceived);	
		pthread_create(&bh262Thread, 	NULL, bh262Loop, NULL);
	}
	//pthread_create(&rxThread, &tattr_server, &Server::RunServer, (void *) this)	



	if(Config.usePPS) initGlut(argc, argv, &MeasG.hdata280, &mutex1);
	
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
	bh280.StopLoop();
	bh262.StopLoop();
	 pthread_join( serverThread, NULL);
	   //pthread_join( bh280Thread, NULL);
	printf("\n");
	return 0;
}

