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
#include "BHSupervisory.h"
#include "app.h"

using namespace std;

SuperServer Server;
BHsupervisory BH280;
BHsupervisory BH262;
Command		ComG;
Reply		RepG;
timeval LastReceived;
pthread_mutex_t mutex1;
pthread_t serverThread, bh280Thread, bh262Thread;

threadMethod serverLoop(void *threadid){Server.Pong();}
threadMethod bh280Loop(void *threadid){BH280.Loop();}
threadMethod bh262Loop(void *threadid){BH262.Loop();}

int main(int argc, char* argv[])
{
	App App;
	Config Config;
	App.PrintBanner();
	Config.FromArgs(argc, argv);
	Config.PrintConf();

	pthread_mutex_init(&mutex1, NULL);
	memset(&ComG,0,sizeof(ComG)); memset(&RepG,0,sizeof(RepG));	
	RepG.executed280=true;

	Server.Init(	Config.port,
					&ComG, &RepG,
					&mutex1, 
					&LastReceived);//(Config);
	
	pthread_create(&serverThread, NULL, serverLoop, NULL);
	
	if (Config.use280){
	BH280.Init(0,ComG.command280,RepG.reply280,&RepG.executed280,
				&mutex1,&LastReceived);
	pthread_create(&bh280Thread, NULL, bh280Loop, NULL);
	}

	if (Config.use262){
		BH262.Init(Config.serialPort,ComG.command262,RepG.reply262,&RepG.executed262,
				&mutex1,&LastReceived);
	pthread_create(&bh262Thread, NULL, bh262Loop, NULL);
	}
	

	string input;
	
	int offset;
	while (true)
	{
		getline(cin,input);
		offset=input.find("q");
		if (offset>-1 && input.size()==1)
		{
		  cout<<"quit\n"<<endl;
		  break;
		}

		offset=input.find("a");
		if (offset>-1 && input.size()==1)
		{
			RepG.executed280=!RepG.executed280;
			cout<<"switch\n"<<endl;
		}
		cout<<endl;
	}
	
	//Server.Stop();
	//bh280.Stop();
	
	
	printf("\n");
	return 0;
}

