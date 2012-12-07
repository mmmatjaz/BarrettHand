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



#include "bh262.h"


BH262::BH262()
{
	props.range[0]=1.0*RANGE_262_G;
	props.range[1]=1.0*RANGE_262_G;
	props.range[2]=1.0*RANGE_262_G;
	props.range[3]=1.0*RANGE_262_S;
	
	props.scaleIN[0]=1.0*RANGE_262_G/(0.7778*PI);
	props.scaleIN[1]=1.0*RANGE_262_G/(0.7778*PI);
	props.scaleIN[2]=1.0*RANGE_262_G/(0.7778*PI);
	props.scaleIN[3]=1.0*RANGE_262_S/(PI);
	
	for (int i=0;i<4;i++)
	{
		props.scaleOUT[i]=1/props.scaleIN[i];
	}

	reg.P[0]=Pg; reg.P[1]=Pg; reg.P[2]=Pg; reg.P[3]=Ps;
	reg.D[0]=Dg; reg.D[1]=Dg; reg.D[2]=Dg; reg.D[3]=Ds;
	reg.maxV[0]=MAX_G; reg.maxV[1]=MAX_G; reg.maxV[2]=MAX_G; reg.maxV[3]=MAX_S;
	reg.maxT[0]=MAX_TG; reg.maxT[1]=MAX_TG; reg.maxT[2]=MAX_TG; reg.maxT[3]=MAX_TS;
	
	memset(&Cons,0,sizeof(Cons));	memset(&Meas,0,sizeof(Meas));
	memset(&pCons,0,sizeof(Cons));	memset(&pMeas,0,sizeof(Meas));
	//bzero(sCache,4*sizeof(StrainCache));
	conn=false;conn_=false;
	ManualValue=100;
	T=0;
}



void BH262::Initialize(	int ComPort,
						struct HandControls * DIN, struct HandMeas * DOUT,
						pthread_mutex_t * mutex,
						timeval * LastReceived)
{
//	copy parameters
	pps=PPS; Cons_=DIN; Meas_=DOUT;
	Mutex=mutex; tstamp=LastReceived; shouldRun=true;

	cout<<("Initialization...")<<endl;
	
	if( result=bh.InitSoftware(ComPort,THREAD_PRIORITY_TIME_CRITICAL) )	Error();
	if( result=bh.ComSetTimeouts(0,100,15000,100,5000) )				Error();
	if( result=bh.Baud(38400) )											Error();
	if (result = bh.InitHand(""))										Error();
	
	//if (result = bh.InitHand("")) Error();
}

void BH262::Loop()
{
/*  *****************************
			hand thread
	***************************** */
	timeval time2;
	
	double dt;
	while(shouldRun)
	{	
		gettimeofday(&time2, NULL);
		pthread_mutex_lock( Mutex );
		dt=diffclock(&time2,tstamp);
		T+=dt;
		pthread_mutex_unlock( Mutex );
	
		if (dt<0.3) conn=true; 	
		else conn=false;
		
		if ( NO_CONN )			usleep(1000);
		if ( CONN_INIT ) 		Begin();
		if ( CONN ) 			RunRealTime();
		if ( CONN_LOST )		TerminateRealTime();
		
		conn_=conn;	
	}
	bh.RTAbort();
	result = bh.Command("t");
	cout<<"Terminating thread bh262\n";
}

void BH262::RunRealTime()
{
	ReadFromHand();
	RefreshData();
	SendToHand();
	RememberData();
}

int BH262::Begin()
{
	cout<<("PrepareRealTime()... ");
	char reply[50];
	int  ctrl=0;
	while(ctrl<1 || ctrl>4) 
	{
		usleep(1000);
		RefreshData();
		ctrl=(int)((Cons.Mode)+0.5);
	}
	
	bh.pCallback = NULL;
	bh.syncMode = BHMODE_SYNC;
	bh.RTAbort();
	if (ctrl==TOR_CONTROL) bh.Command("2M 50000");
	bool LCV = true;          // loop control velocity
	int LCVC = 1;			  // loop control velocity coefficient
	bool LCPG = true;         // loop control proportional gain
	bool LCT = false;         // loop control torque

	bool LFV = true;          // loop feedback velocity
	int  LFVC = 1;            // loop feedback velocity coefficient
	bool LFS = false;         // loop feedback strain
	bool LFAP = true;         // loop feedback absolute position
	bool LFDP = false;        // loop feedback delta position
	int  LFDPC = 1;           // loop feedback delta position coefficient

	bool LFBP = false;        // loop feedback breakaway position not needed
	bool LFAIN = false;       // loop feedback analog inputs not needed
	bool LFDPD = false;       // loop feedback delta position discard not needed
	bool LFT = false;         // loop feedback temperature not needed

	if (result = bh.Command("GFSET DP 100000"))
		Error();
	// Set RealTime mode flags
	if ((bh.RTSetFlags("123S",	LCV, LCVC, LCPG, LCT,LFV, 
		LFVC, LFS, LFAP, LFDP, LFDPC, LFBP, LFAIN, LFDPD, LFT)))
		Error();
	bh.Command("1fset tstop 10000");bh.Command("2fset tstop 10000");
	bh.Command("3fset tstop 10000");bh.Command("sset tstop 10000");
	bh.Command("3fsave");

	switch(ctrl)
	{
		case VEL_CONTROL:	// velocity control
			cout<<"PrepareRealTime()... Velocity mode "<<endl;
			if (result = bh.Set("123S", "LCV", true))
				Error();
			if (result = bh.Set("123S", "LCP", false))
				Error();
			if (result = bh.Set("123S", "LCT", false))
				Error();
			break;

		case POS_CONTROL:	// position control
			cout<<"PrepareRealTime()... Position mode "<<endl;
			if (result = bh.Set("123S", "LCV", false))
				Error();
			if (result = bh.Set("123S", "LCP", true))
				Error();
			if (result = bh.Set("123S", "LCT", false))
				Error();
			break;
		
		case TOR_CONTROL:	// torque control			
			cout<<"PrepareRealTime()... Torque mode "<<endl;					
			if (result = bh.Set("123S", "LCV", false))
				;
			if (result = bh.Set("123S", "LCP", false))
				;
			if (result = bh.Set("123S", "LCT", true))
				;
			break;

		case CUSTOM_CONTROL:	//	set one of the bellow flags
			if (result = bh.Set("123S", "LCV", false))	//velocity
				;
			if (result = bh.Set("123S", "LCP", false))	//position
				;
			if (result = bh.Set("123S", "LCT", true))	//torque
				;
			break;
	}
	
	cout<<("RTStart()....");

	if (result = bh.RTStart("123S", BHMotorTorqueLimitProtect)) 
		Error();
	else cout<<("OK\n");

	bh.RTUpdate();
	
	return 0;
}

void BH262::SendToHand()
{
	gettimeofday(&now, NULL);
	int temp;
	
	int ctrl=(int)(Cons.Mode+0.5);
	// set data to hand
	switch (ctrl)
	{
	case VEL_CONTROL:	// velocity control		
		for (int m = 0; m < 4; m++)
		{
			temp=(int)((Cons.cValues[m] * props.scaleIN[m])+0.5);
			cout<<"\nvel: "<<Cons.cValues[m]<<"  Pos: "<<Meas.Position[m];
			result=bh.RTSetVelocity(m + '1', temp);
		}
		break;
	case POS_CONTROL:	// position control
		for (int m = 0; m < 4; m++)
		{	
			temp=(int)((Cons.cValues[m] * props.scaleIN[m])+0.5);
			//cout<<"\nVel: "<<Cons.cValues[m]<<"  Pos: "<<Meas.Position[m];
			result=bh.RTSetPosition(m + '1', temp);
		}
		break;

	case TOR_CONTROL:	// torque control
		for (int m = 0; m < 4; m++)
		{	
			result=bh.RTSetTorque(m + '1', (int)Cons.cValues[m]);
			cout<<"\nTor: "<<Cons.cValues[m]<<"  Pos: "<<Meas.Position[m];
		}
		break;
	case CUSTOM_CONTROL:
		for (int m = 0; m < 4; m++)
		{
			temp=(int)((PositionControlC(m) * props.scaleIN[m])+0.5);
			result=bh.RTSetVelocity(m + '1', temp);
		}
		break;
	}
	bh.RTUpdate();
}

void BH262::TerminateRealTime()
{
	cout<<"\nDisconnecting\n"<<endl;
	bh.RTAbort();
	result = bh.Command("t");
}

void BH262::StopLoop()
{
	bh.RTAbort();
	result = bh.Command("t");
	shouldRun=false;
}
double BH262::TorqueControlPD(int m)
{
  double out=0.0;
  double dt=diffclock(&now,&lc);
  double Kf=0.5/10.0;
  //P
	double qd=Cons.cValues[m];
	double q= Meas.Position[m];
	double e= qd-q;
  //D
	double pqd=pCons.cValues[m];
	double pq= pMeas.Position[m];
	double pe= pqd-pq;
	
	double de=(e-pe)/dt;
  
	out=900*e+15*de;// + reg.D[m]*de;

	
 if (out>reg.maxT[m]) 		out=reg.maxT[m];
 if (out<-reg.maxT[m]) 	out=-reg.maxT[m];
	
 Meas.DeltaPos[m]=out;
  return out;

}

double BH262::PositionControl(int m)
{
	LowPass(m,0.3);
  double out=0.0;
  double dt=diffclock(&now,&lc);
  double Kf=0.5/10.0;
  //P
	double qd=Cons.cValues[m];
	double qf=Kf*Deadzone(m,0.30);
	double q= Meas.Position[m];
	double e= qd-q;//-qf;
	//Meas.m280[2][m]=qd-q-qf;
  //D
	double pqd=pCons.cValues[m];
	double pqf;//=Kf*SmoothDeadzone(pMeas.StrainFilt[m]);
	double pq= pMeas.Position[m];
	double pe= pqd-pqf-pq;
	double de=pe/dt;
  out=reg.P[m]*e;// + reg.D[m]*de;

  if (out>reg.maxV[m]) 		out=reg.maxV[m];
  if (out<-reg.maxV[m]) 	out=-reg.maxV[m];
 
  return out;
}

double BH262::PositionControlC(int m)
{
	LowPass(m,0.3);
  double out=0.0;
  double dt=diffclock(&now,&lc);
  double Kf=0.5/20.0;
  //P
	double qd=Cons.cValues[m];
	double qf=Kf*Deadzone(m,0.30);
	double q= Meas.Position[m];
	double e= qd-q-qf;
	//Meas.m280[2][m]=qd-q-qf;
  //D
	double pqd=pCons.cValues[m];
	double pqf=Kf*Deadzone(m,0.30);
	double pq= pMeas.Position[m];
	double pe= pqd-pqf-pq;
	double de=pe/dt;
  out=reg.P[m]*e;// + reg.D[m]*de;

  if (out>reg.maxV[m]) 		out=reg.maxV[m];
  if (out<-reg.maxV[m]) 	out=-reg.maxV[m];
 
  return out;
}

double BH262::VelocityControl(int m)
{
	// r = velocity, u = velocity (speed limits only)
  double out=0.0;
  out=Cons.cValues[m];
  
  if (out>reg.maxV[m]) 		out=reg.maxV[m];
  if (out<-reg.maxV[m]) 	out=-reg.maxV[m];
  return out;
}

void BH262::LowPass(int m,double a)
{
  double dt=diffclock(&now,&lc);
  double x =( Meas.Strain[m]-126);
  double y_=(pMeas.StrainFilt[m]);
  double y=x*dt/(a+dt)+y_*a/(a+dt);
  Meas.StrainFilt[m]=y;
  //cout<<x<<" "<<y<<endl;
}

double BH262::Deadzone(int m, double dz)
{
	double  x= (Meas.StrainFilt[m]);
	double px=(pMeas.StrainFilt[m]);
	double dx=x-px;
	//cout << x-px << ", ";
	if (dx<dz && dx>-dz) 
	return px;
	else return x;
}
void BH262::ReadFromHand()
{
	for (int m = 0; m < 4; m++)//! 3->4
	{	
		Meas.Position[m] = bh.RTGetPosition(m + '1')*props.scaleOUT[m];
		Meas.Velocity[m] = bh.RTGetVelocity(m + '1')*props.scaleOUT[m];
		Meas.DeltaPos[m] = bh.RTGetDeltaPos(m + '1')*props.scaleOUT[m];
		Meas.Strain[m] = bh.RTGetStrain(m + '1');
		if (pps)
			bh.RTGetPPS(m + '1', &Meas.pps[m][0], MAX_PPS_ELEMENTS);	
	}
}
void BH262::RefreshData()
{
	pthread_mutex_lock( Mutex );		
	memcpy(	(char *)&(Cons),
			(char *)(Cons_),	
			sizeof(HandControls));	
			
	memcpy(	(char *)Meas_, 	
			(char *)&Meas, 	
			sizeof(HandMeas));
	
	if (pps)
	 memcpy((char *)Meas_->pps,
			(char *)Meas.pps,
			sizeof(Meas.pps));	
	
	pthread_mutex_unlock( Mutex );
}
void BH262::RefreshMeas()
{
	pthread_mutex_lock( Mutex );				
	memcpy(	(char *)Meas_, 	
			(char *)&Meas, 	
			sizeof(HandMeas));
	if (pps)
	 memcpy((char *)Meas_->pps,
			(char *)Meas.pps,
			sizeof(Meas.pps));	
	pthread_mutex_unlock( Mutex );
}
void BH262::RememberData()
{
	memcpy(	(char *)&pCons,
			(char *)&Cons,	
			sizeof(HandControls));			
	memcpy(	(char *)&pMeas, 	
			(char *)&Meas, 	
			sizeof(HandMeas));
	memcpy(	&lc,&now,sizeof(now));	
}


void BH262::Error()
{
	sprintf(bufy,"ERROR: %d\n%s\n", result, bh.ErrorMessage(result));
	cout<<bufy<<endl;
	//exit(0);
}

double BH262::diffclock(timeval* currentTime, timeval* startTime)
{
	double t2=(double)currentTime->tv_sec+(double)currentTime->tv_usec/1000000.0;
	double t1=(double)startTime  ->tv_sec+(double)startTime  ->tv_usec/1000000.0;
	if (t2-t1 > 0.0)
		return t2-t1;
	else
		return 0.0;
}