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

#ifndef BH280_H
#define BH280_H

#define MODEL262	1
#define MODEL280	2

#include "app.h"
#include "BHand.h"
#include "BHandAppHelper.h" // for connection menu

using namespace std;


class BHsupervisory
{
private:
//local data
	//current
	timeval now;
//pointers
	bool * Executed;
	char * Command;
	char * Reply;
	timeval * tstamp;
	pthread_mutex_t * Mutex;
//properties
	int Hand; //0 is b280, else 262 @ com
	bool shouldRun;
	pthread_t thread;
//api
	BHand bh;
	int result;
	char bufy[100];
//methods
	void WaitForNewCommand();
	void ApplyAndRespond();
	void Error();
	double diffclock(timeval* currentTime, timeval* startTime);
public:
	//BHsupervisory();
	void Init(  int hand,
					char * command, char * reply, bool * executed,
					pthread_mutex_t * mutex,
					timeval * LastReceived);
	void Stop();
	threadMethod Loop();
};
#endif