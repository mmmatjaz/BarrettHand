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

static bool shouldRun=true;

threadMethod bh280Loop(void *threadid)
{	
	bh280.LoopOfflineExample();
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

	//initGlut(argc, argv, &MeasG.hdata280, &mutex1);

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
			shouldRun=false;
			cout<<"quit\n"<<endl;
			bh280.StopLoop();
			break;
		}	
		cout<<endl;
	}
	
	
	bh280.Stop();
	
	//pthread_join( bh280Thread, NULL);
	printf("\n");
	return 0;
}

