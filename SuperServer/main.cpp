
#include "server.h"
#include "bh280.h"
#include "app.h"

using namespace std;

Command ComG;
Reply	RepG;
timeval LastReceived;
pthread_mutex_t mutex1;

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
	SuperServer Server(	Config.port,
						&ComG, &RepG,
						&mutex1, 
						&LastReceived);//(Config);
	BHsupervisory bh280(0,ComG.command280,RepG.reply280,&RepG.executed280,
						&mutex1,&LastReceived);
	Server.Start();
	bh280.Start();
	
	string input;
	string tmp;
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
	
	Server.Stop();
	bh280.Stop();
	
	
	printf("\n");
	return 0;
}

