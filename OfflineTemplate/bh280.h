#ifndef BH280_H
#define BH280_H

#include "app.h"
#include "BHand.h"
#include "BHandAppHelper.h" // for connection menu

using namespace std;

struct Props
{
	double scaleOUT[4];
	double scaleIN[4];
	double range[4];
};

struct Regulator
{
	double maxV[4];
	double maxT[4];
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
	bool realtime;
//par	
	Props props;
	Regulator reg; 
//api
	BHand bh;
	int result;
	char bufy[100];
//methods
	int Begin();
	double TorqueControlPD(int m);
	double PositionControlC(int m);
	double PositionControl(int m);
	double VelocityControl(int m);
	void LowPass(int m,double a);
	double Deadzone(int m, double dz);
	void ReadFromHand();
	void RefreshData();
	void RememberData();
	void SendToHand();
	void Error();
	double diffclock(timeval* currentTime, timeval* startTime);
public:
	int ManualValue;
	BH280();
	void Initialize(bool PPS,
					struct HandControls * DIN, struct HandMeasPPS * DOUT,
					pthread_mutex_t * mutex,
					timeval * LastReceived);
	void Stop();
	void LoopOfflineVelocity();
	void LoopOfflinePosition();
	void LoopOfflineTorque();
	void StopLoop();
	
};
#endif