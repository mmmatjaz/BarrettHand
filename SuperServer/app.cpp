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
	printf("\n   0   0         SUPER UDP SERVER   ");
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
	port=	4444;
	use280=true;
	use262=false;
	
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
	cout<<endl;
	if (use262) cout<<"using bh262 @com"<<serialPort<<endl;
}

