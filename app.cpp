#include "app.h"

using namespace std;

/*
App::App()
{
}
*/

void App::PrintBanner()
{
	//printf("\x1b[%d;%dH%O",0,0);
	printf("\n");
	printf("\n     0 0               IJS             ");
	printf("\n   0 0             BARRETT HAND         ");
	printf("\n   0   0   VERY REAL-TIME UDP SERVER   ");
	printf("\n     0                             \n\n");
}

double App::DiffClock(timeval* currentTime, timeval* startTime)
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


Config::Config()
{
	RXport= 4444;
	TXport=	5555;
	use280=true;
	use262=false;
	usePPS=true;
	serialPort=1;
	ST=		0.01;
};

void Config::Usage()
{
	printf("\tBad arguments. See manual\n");
}

void Config::FromArgs(int argc, char* argv[])
{
/*
	for (int i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-rx")==0) 	{strcpy(IP,argv[i+1]  );mode=MODE_LAME;};
		if(strcmp(argv[i],"-tx" )==0)	port=	atoi(argv[i+1]);
		if(strcmp(argv[i],"-m" )==0)	mode=	atoi(argv[i+1]);
		if(strcmp(argv[i],"-f" )==0)	signals=atoi(argv[i+1]);
		if(strcmp(argv[i],"-st")==0)	ST	=	atof(argv[i+1]);
				//printf("\n");
	}
	if (mode==0 || port==0)
	{
		Usage();
		exit(0);
	}
	*/
}

void Config::PrintConf()
{
	cout<<"RX"<<RXport<<" TX"<<TXport<<endl;
	if (use280) cout << "using bh280";
	if (usePPS && use280) cout << " with PPS\n"; else cout<<endl;
	if (use262) cout<<"using bh280 @com"<<serialPort<<endl;
}

