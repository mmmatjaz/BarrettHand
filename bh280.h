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


struct FilterCache
{
	double  y_0;
	double  y_1;
	double 	dt;
};

class BH280
{
private:
//local data
	//current
	timeval now;
	HandControls Cons;
	HandMeasPPS Meas;
	//last cycle
	timeval lc;
	HandControls pCons;
	HandMeasPPS pMeas;
//pointers
	HandControls * Cons_;
	HandMeasPPS * Meas_;
	timeval * tstamp;
	pthread_mutex_t * Mutex;
//flags
	bool pps;
	bool shouldRun;
	bool conn;
	bool conn_;
//par	
	Props props;
	Regulator reg; 
//api
	BHand bh;
	int result;
	char bufy[100];
//methods
	int PrepareRealTime();
	double PositionControlC(int m);
	double VelocityControl(int m);
	void LowPass(int m,double a);
	void LowPass2(int m,double aa);
	double LowPassAny(double x, double y_, double a);
	double SmoothDeadzone(double value);
	double Deadzone(int m, double dz);
	void RunRealTime();
	void ReadFromHand();
	void RefreshData();
	void RefreshMeas();
	void RememberData();
	void SendToHand();
	void Error();
	double diffclock(timeval* currentTime, timeval* startTime);
public:
	BH280();
	void Initialize(bool PPS,
					struct HandControls * DIN, struct HandMeasPPS * DOUT,
					pthread_mutex_t * mutex,
					timeval * LastReceived);
	void Stop();
	void Loop();
	void LoopOffline();
	void LoopOfflineTorque();
};
#endif