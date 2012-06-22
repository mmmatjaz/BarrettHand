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


#include "client.h"
#include "app.h"

using namespace std;

App		App;
Client	Client;

Controls  ConsG;
Measurements MeasG;
timeval LastReceived;

pthread_mutex_t mutex1;

int main(int argc, char* argv[])
{
	App.PrintBanner();

	pthread_mutex_init(&mutex1, NULL);
	memset(&ConsG,0,sizeof(ConsG)); memset(&MeasG,0,sizeof(MeasG));	

	Client.Init(	4444, 4444,
					(char *) &ConsG, (char *) &MeasG,
					sizeof(ConsG),sizeof(MeasG),
					&mutex1, &LastReceived,
					0.01);//(Config);
	
	// set position mode
	ConsG.con262.Mode=0;
	ConsG.con280.Mode=0;
	
	// set starting reference
	for (int i=0;i<4;i++)
	{
		ConsG.con262.cValues[i]=0;
		ConsG.con280.cValues[i]=0;
	}
	// send initital reference
	Client.Send();
	
	while (true)
	{
		//receive measurements
		Client.Receive();
		//set new position reference
		ConsG.con280.cValues[0]=1;
		//.........
		//....
		//send controls
		Client.Send();
	}
	
	//pthread_join( bh280Thread, NULL);
	cout<<endl;
	return 0;
}

