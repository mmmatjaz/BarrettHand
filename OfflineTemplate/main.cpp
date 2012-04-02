#include "bh280.h"
#include "app.h"
#include "graphics.h"

using namespace std;

App		App;
BH280	bh280;

Controls  ConsG;
Measurements MeasG;
timeval LastReceived;

pthread_mutex_t mutex1;
pthread_t bh280Thread;


threadMethod bh280Loop(void *threadid)
{	
	//bh280.Loop();
	bh280.LoopOffline();
}

int main(int argc, char* argv[])
{
	
	App.PrintBanner();

	pthread_mutex_init(&mutex1, NULL);
	memset(&ConsG,0,sizeof(ConsG)); memset(&MeasG,0,sizeof(MeasG));	
		
	bh280.Initialize(false,
					&ConsG.con280,&MeasG.hdata280,
					&mutex1,
					&LastReceived);	
	
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
	
	
	bh280.Stop();
	
	//pthread_join( bh280Thread, NULL);
	printf("\n");
	return 0;
}

