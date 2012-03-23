
#include "server.h"
#include "BHSupervisory.h"
#include "app.h"

using namespace std;

SuperServer Server;
BHsupervisory BH280;
BHsupervisory BH262;
Command ComG;
Reply	RepG;
timeval LastReceived;
pthread_mutex_t mutex1;
pthread_t serverThread, bh280Thread, bh262Thread;

threadMethod serverLoop(void *threadid){Server.Pong();}
threadMethod bh280Loop(void *threadid){BH280.Loop();}
//threadMethod bh262Loop(void *threadid){BH262.Pong();}

int main(int argc, char* argv[])
{
	App App;
	Config Config;
	App.PrintBanner();
	Config.FromArgs(argc, argv);
	Config.PrintConf();

	pthread_mutex_init(&mutex1, NULL);
	memset(&ComG,0,sizeof(ComG)); memset(&RepG,0,sizeof(RepG));	
	RepG.executed280=false;

	Server.Init(	Config.port,
					&ComG, &RepG,
					&mutex1, 
					&LastReceived);//(Config);
	
	BH280.Init(0,ComG.command280,RepG.reply280,&RepG.executed280,
				&mutex1,&LastReceived);
	pthread_create(&serverThread, NULL, serverLoop, NULL);
	if (Config.use280) pthread_create(&bh280Thread, NULL, bh280Loop, NULL);
	
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

