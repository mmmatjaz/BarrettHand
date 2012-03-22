#ifndef BH280_H
#define BH280_H

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
	BHsupervisory(  int hand,
					char * command, char * reply, bool * executed,
					pthread_mutex_t * mutex,
					timeval * LastReceived);
	void Stop();
	void *Loop();
	void Start();
	static void* Run(void *ptr){return ((BHsupervisory *) ptr)->Loop(); };
};
#endif