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
	port= 4444;
	use280=true;
	use262=false;
	usePPS=false;
	serialPort=1;
	ST=		0.01;
};

void Config::Usage()
{
	printf("\tBad arguments - using defaults. See manual\n");
}

void Config::FromArgs(int argc, char* argv[])
{
	if (argc==4)
	{
		port=atoi(argv[1]);
		use280=atoi(argv[2])==1 ? true:false;
		usePPS=atoi(argv[2])==2 ? true:false;
		use262=atoi(argv[3])>0 ? true:false;
		serialPort=atoi(argv[3]);
	}
	if (argc!=4 && argc!=1)
	{
		Usage();
	}
}

void Config::PrintConf()
{
	cout<<"Port "<<port<<endl;
	if (use280) cout << "using bh280";
	if (usePPS && use280) cout << " with PPS\n"; else cout<<endl;
	if (use262) cout<<"using bh262 @com"<<serialPort<<endl;
}

