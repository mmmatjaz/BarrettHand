#ifndef BHRT_H
#define BHRT_H

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

class bhRT
{
private:
//local data
	double T;
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
	int ComPort;
	int Model;
	Props props;
	Regulator reg; 
//api
	BHand bh;
	int result;
	char bufy[100];
//methods
	int Begin();
	void TerminateRealTime();
	double TorqueControlPD(int m);
	double PositionControlC(int m);
	double PositionControl(int m);
	double VelocityControl(int m);
	void LowPass(int m,double a);
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
	int ManualValue;
	bhRT(int model);
	void Initialize(bool PPS,
					struct HandControls * DIN, struct HandMeasPPS * DOUT,
					pthread_mutex_t * mutex,
					timeval * LastReceived);
	void Stop();
	void Loop();
	//void LoopOfflineVelocity();
	//void LoopOfflinePosition();
	//void LoopOfflineTorque();
	void StopLoop();
};
#endif