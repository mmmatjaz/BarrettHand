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
	
	author: Matja� Ogrinc
			matjaz.ogrinc42@gmail.com
			https://github.com/mmmatjaz
			
			IJS 2011-2012
			abr.ijs.si
*/	


#ifndef BH280_H
#define BH280_H

#define POS_SCALE_280_S 1.0
#define POS_SCALE_280_G 1.0
#define TOR_SCALE_280_S 1.0
#define TOR_SCALE_280_G 1.0

#define RANGE_280_S 35840
#define RANGE_280_G 199000
#define PI			4.0*atan(1.0)

#define Pg 0.05
#define Ps 0.001
#define Dg 0//.001
#define Ds 0//.001
#define MAX_G 0.004
#define MAX_S 0.001
#define MAX_TG 1100
#define MAX_TS 100

#define VEL_CONTROL		1
#define POS_CONTROL		2
#define TOR_CONTROL		3
#define CUSTOM_CONTROL	4

#define NO_CONN		!conn_ && !conn
#define CONN		 conn_ &&  conn
#define CONN_LOST	 conn_ && !conn
#define CONN_INIT	!conn_ &&  conn

#include "app.h"
#include "BHand.h"
//#include "/usr/local/Barrett/BHand/API/BHand.h"
#include "BHandAppHelper.h" // for connection menu

using namespace std;



class BH280
{
private:
//local data
	double dt;
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
	BH280();
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
