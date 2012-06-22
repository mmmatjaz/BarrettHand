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
	printf("\n   0   0								");
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
	if (use262) cout<<"using bh262 @com"<<serialPort<<endl;
}

