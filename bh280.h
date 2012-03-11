#ifndef BH280_H
#define BH280_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <deque>
#include "BHand.h"
#include "BHandAppHelper.h" // for connection menu
#include <sys/time.h>// Needed for ms time.
#include <time.h>// Needed for ms time.
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <iostream>

#include "app.h"

using namespace std;

struct Props
{
	double scaleIN[4];
	double scaleOUT[4];
	double range[4];
};

struct Regulator
{
	double maxV[4];
	double P[4];
	double D[4];
};

class BH280
{
private:
//data
	struct Controls din;
	struct Measurements dout;
	
	struct Controls * din_;
	struct Measurements * dout_;
	timeval * tstamp;
	pthread_mutex_t * Mutex;
//flags
	bool pps;
	bool shouldRun;
	bool conn;
	bool conn_;
//par	
	struct Props props;
	struct Regulator reg; 
//api
	BHand bh;
	int result;
	char bufy[100];
//methods
	double diffclock(timeval* currentTime, timeval* startTime);
	void Error();
	int PrepareRealTime();
	double Regulate(int m);
	void RunRealTime();
public:
	BH280();
	void Initialize(bool PPS,
					struct Controls * DIN, struct Measurements * DOUT,
					pthread_mutex_t * mutex,
					timeval * LastReceived);
	void Stop();
	void Loop();
	void LoopTest();
	void PrepareRT();
};
#endif