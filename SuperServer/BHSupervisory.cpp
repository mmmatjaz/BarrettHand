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


#define SIGNATURE "BH"

#include "BHSupervisory.h"


void BHsupervisory::Error()
{
	sprintf(bufy,"ERROR: %d\n%s\n", result, bh.ErrorMessage(result));
	cout<<bufy<<endl;
	//exit(0);
}

void BHsupervisory::Init(	int hand,
						char * command, char * reply, bool * executed,
						pthread_mutex_t * mutex,
						timeval * LastReceived)
{
//	copy parameters
	Hand=hand;
	Command=command; Reply=reply; Executed=executed;
	Mutex=mutex; tstamp=LastReceived; shouldRun=true;

	int hwIndex = BHandHardware::getBHandHardwareIndex("BH8-280");
	if (hwIndex < 0)
	{
		cout<<("The API has not been compiled to include target hand.");
			Error();
	}
	bh.setHardwareDesc(hwIndex);
	bool use280Config = (strcmp(bh.getHardwareDesc()->getModelNumber(), "BH8-280") == 0);
	
	cout<<("Initializing hand...")<<endl;
	
	if (hand==0)	
	{
		bh.Init(0, THREAD_PRIORITY_TIME_CRITICAL, BH_CAN_COMMUNICATION, true);
		bh.InitHand("");
	}else
	{
		if( result=bh.InitSoftware(hand,THREAD_PRIORITY_TIME_CRITICAL) )	Error();
		if( result=bh.ComSetTimeouts(0,100,15000,100,5000) )				Error();
		if( result=bh.Baud(38400) )											Error();
		if (result = bh.InitHand(""))										Error();

	
}


threadMethod BHsupervisory::Loop()
{
/*  *****************************
			hand thread
	***************************** */
	timeval time2;
	cout<<"Hand thread started"<<endl;
	while(shouldRun)
	{	
		WaitForNewCommand();
		ApplyAndRespond();	
	}
	bh.RTAbort();
	result = bh.Command("t");
	cout<<"Terminating thread bh280\n";
}

void BHsupervisory::WaitForNewCommand()
{
	bool wait=true;
	while (wait)
	{
		usleep(100000);
		pthread_mutex_lock( Mutex );
		wait=(*Executed);
		pthread_mutex_unlock( Mutex );
	}
	usleep(100000);
}

void BHsupervisory::ApplyAndRespond()
{
	char command[4*8];
	char reply[10*8];
	pthread_mutex_lock( Mutex );
	strcpy(command,Command);
	pthread_mutex_unlock( Mutex );

	bh.Command(command,reply);

	pthread_mutex_lock( Mutex );
	strcpy(Reply,reply);
	*Executed=true;
	pthread_mutex_unlock( Mutex );

	result = bh.Command("t");
	cout<< SIGNATURE <<" Command: "	<<Command;
	cout<< " Reply: "	<<Reply<<endl;
	
	
}

double BHsupervisory::diffclock(timeval* currentTime, timeval* startTime)
{
	double t2=(double)currentTime->tv_sec+(double)currentTime->tv_usec/1000000.0;
	double t1=(double)startTime  ->tv_sec+(double)startTime  ->tv_usec/1000000.0;
	if (t2-t1 > 0.0)
		return t2-t1;
	else
		return 0.0;
}


void BHsupervisory::Stop()
{
	bh.RTAbort();
	result = bh.Command("t");
	shouldRun=false;
	pthread_cancel(thread);
	cout<<"Server thread killed"<<endl;
}