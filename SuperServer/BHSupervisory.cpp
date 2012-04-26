#define SIGNATURE "BH280"

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
	bh.Init(0, THREAD_PRIORITY_TIME_CRITICAL, BH_CAN_COMMUNICATION, true);
		bh.InitHand("");

	
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