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

#include "bh280.h"


BH280::BH280()
{
	props.range[0]=1.0*RANGE_280_G;
	props.range[1]=1.0*RANGE_280_G;
	props.range[2]=1.0*RANGE_280_G;
	props.range[3]=1.0*RANGE_280_S;
	
	props.scaleIN[0]=1.0*RANGE_280_G/(0.7778*PI);
	props.scaleIN[1]=1.0*RANGE_280_G/(0.7778*PI);
	props.scaleIN[2]=1.0*RANGE_280_G/(0.7778*PI);
	props.scaleIN[3]=1.0*RANGE_280_S/(PI);
	
	for (int i=0;i<4;i++)
	{
		props.scaleOUT[i]=1/props.scaleIN[i];
	}
	
	reg.P[0]=Pg; reg.P[1]=Pg; reg.P[2]=Pg; reg.P[3]=Ps;
	reg.D[0]=Dg; reg.D[1]=Dg; reg.D[2]=Dg; reg.D[3]=Ds;
	reg.maxV[0]=MAX_G; reg.maxV[1]=MAX_G; reg.maxV[2]=MAX_G; reg.maxV[3]=MAX_S;
	
	memset(&Cons,0,sizeof(Cons));	memset(&Meas,0,sizeof(Meas));
	memset(&pCons,0,sizeof(Cons));	memset(&pMeas,0,sizeof(Meas));
	//bzero(sCache,4*sizeof(StrainCache));
	conn=false;conn_=false;
}

void BH280::Error()
{
	sprintf(bufy,"ERROR: %d\n%s\n", result, bh.ErrorMessage(result));
	cout<<bufy<<endl;
	//exit(0);
}

void BH280::Initialize(	bool PPS,
						struct HandControls * DIN, struct HandMeasPPS * DOUT,
						pthread_mutex_t * mutex,
						timeval * LastReceived)
{
//	copy parameters
	pps=PPS; Cons_=DIN; Meas_=DOUT;
	Mutex=mutex; tstamp=LastReceived; shouldRun1=true; shouldRun2=true;

	int hwIndex = BHandHardware::getBHandHardwareIndex("BH8-280");
	if (hwIndex < 0)
	{
		cout<<("The API has not been compiled to include target hand.");
			Error();
	}
	bh.setHardwareDesc(hwIndex);
	bool use280Config = (strcmp(bh.getHardwareDesc()->getModelNumber(), "BH8-280") == 0);
	cout<<("Initialization...")<<endl;
	if (result = handInitWithMenu(&bh)) Error();	
	if (result = bh.InitHand("")) Error();
	else ;//printC("Done",BHAND280);	
	if (pps) if(result = bh.Set("GS","LFPPS",1)) ;//Error();
	
}

double BH280::PositionControl(int m)
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
	double pqf=Kf*Deadzone(m,0.30);
	double pq= pMeas.Position[m];
	double pe= pqd-pqf-pq;
	double de=pe/dt;
  out=reg.P[m]*e;// + reg.D[m]*de;

  if (out>reg.maxV[m]) 		out=reg.maxV[m];
  if (out<-reg.maxV[m]) 	out=-reg.maxV[m];
 
  return out;
}

double BH280::PositionControlC(int m)
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

double BH280::VelocityControl(int m)
{
	// r = velocity, u = velocity (speed limits only)
  double out=0.0;
  out=Cons.cValues[m];
  
  if (out>reg.maxV[m]) 		out=reg.maxV[m];
  if (out<-reg.maxV[m]) 	out=-reg.maxV[m];
  return out;
}

void BH280::LowPass(int m,double a)
{
  double dt=diffclock(&now,&lc);
  double x =( Meas.Strain[m]-126);
  double y_=(pMeas.StrainFilt[m]);
  double y=x*dt/(a+dt)+y_*a/(a+dt);
  Meas.StrainFilt[m]=y;
  //cout<<x<<" "<<y<<endl;
}

double BH280::Deadzone(int m, double dz)
{
	double  x= (Meas.StrainFilt[m]);
	double px=(pMeas.StrainFilt[m]);
	double dx=x-px;
	//cout << x-px << ", ";
	if (dx<dz && dx>-dz) 
	return px;
	else return x;
}

void BH280::LoopOfflineVelocity()
{
/*  *****************************
			hand thread
	***************************** */
	shouldRun1=true;
	timeval Tstart;
	timeval Tnow;
	timeval Tbefore;
	double T;

	if (result = bh.Command("GFSET DP 100000"))
		Error();
	if (result = bh.RTSetFlags( "1234", 1, 3, 0, 0, 1, 1, 1, 1, 1, 1,0,0,0,0 ))
		Error();
	if (result = bh.Set("123S", "LCV", true))
		Error();
	if (result = bh.Set("123S", "LCP", false))
		Error();
	if (result = bh.Set("123S", "LCT", false))
		Error();
	if (result = bh.RTStart( "1234" )) 
		Error();

	bh.RTUpdate();

	T=0.0;
	gettimeofday(&lc, NULL);
	while (shouldRun1)
	{	
		gettimeofday(&now, NULL);
		T+=diffclock(&now,&lc);
		ReadFromHand();

		double f=0.2;//Hz
		double value = 0.8*PI/3 + (PI/4*sin( f* T*2*PI));

		if(!pps)
		{
			int m=1;
			Cons.cValues[m]=value;			
			int temp=(int)((PositionControlC(m) * props.scaleIN[m])+0.5);
			result=bh.RTSetVelocity(m + '1', temp);
			m=0;
			Cons.cValues[m]=value;	
			temp=(int)((PositionControl(m) * props.scaleIN[m])+0.5);
			result=bh.RTSetVelocity(m + '1', temp);

			m=3;
			Cons.cValues[m]=1;	
			temp=(int)((PositionControl(m) * props.scaleIN[m])+0.5);
			result=bh.RTSetVelocity(m + '1', temp);
			
		}
		bh.RTUpdate();
		RememberData();
	}
	bh.RTAbort();
	result = bh.Command("t");
	cout<<"Terminating thread bh280\n";
}

void BH280::LoopOfflineTorque()
{
/*  *****************************
			hand thread
	***************************** */
	shouldRun2=true;
	timeval Tstart;
	timeval Tnow;
	timeval Tbefore;
	double T;
	
	bh.pCallback = NULL;
	bh.syncMode = BHMODE_SYNC;
	bh.RTAbort();

	if (result = bh.Command("GFSET DP 100000"))
		Error();
	if (result = bh.RTSetFlags( "1234", 1, 3, 0, 0, 1, 1, 1, 1, 1, 1,0,0,0,0 ))
		Error();
	if (result = bh.Set("123S", "LCV", false))
		Error();
	if (result = bh.Set("123S", "LCP", false))
		Error();
	if (result = bh.Set("123S", "LCT", true))
		Error();
	if (result = bh.RTStart( "1234" )) 
		Error();

	bh.RTUpdate();
	T=0.0;
	gettimeofday(&lc, NULL);

	cout<<"grem v loop"<<endl;
	int navor=0;

	while (shouldRun2)
	{	

		gettimeofday(&now, NULL);
		T+=diffclock(&now,&lc);
		ReadFromHand();
		double f=0.2;//Hz
		double reference = 0.8*PI/3 + (PI/4*sin( f* T*2*PI));

		double torPGainFinger = 500;
		double torDGainFinger = 3.0;
		// Spread Gains
		double torPGainSpread = 2400/2;
		double torDGainSpread = 36/2;
		double torque;
		int temp=0;
		for (int i = 0; i < 3; i++)
		{
			/*
			if (i < 3)
			{
				// Original torque control method
				int actualVel = Meas.Velocity[i];
				float posError = Meas.Position[i] - reference;
				if (posError > 3.14f/2)
					posError = 3.14f/2;
				else if (posError < -3.14/2)
					posError = -3.14f/2;
				torque = posError * torPGainFinger;
				torque += actualVel * torDGainFinger;
			}
			else
			{
				//int actualVel = m_hand->RTGetVelocity('1' + i);
				//torque = (ap[i] - opengl_pos[i]) * torPGainSpread;
				//torque += actualVel * torDGainSpread;
			}

			if (result = bh.RTSetTorque('1' + i, -(int)torque))
					Error();
		
			*/
			cout<<Meas.Position[i]<< ", ";
			if (Meas.Position[i]<0.5)
			{
				if (Meas.Velocity[i]<1)
				{
					navor++;
					cout<<"povecujem ";
				}
				cout<<navor<<endl;
				result = bh.RTSetTorque('1'+i, navor);
			}
			else result = bh.RTSetTorque('1'+i, 0);
		}
		cout<<"\r";
		bh.RTUpdate(true, true);
		//bh.RTUpdate();
		
		RememberData();
	}
	bh.RTAbort();
	result = bh.Command("t");
	cout<<"Terminating thread bh280\n";
}

void BH280::Stop()
{
	bh.RTAbort();
	result = bh.Command("t");
	//shouldRun=false;
}
void BH280::ReadFromHand()
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

void BH280::RefreshData()
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

void BH280::RememberData()
{
	memcpy(	(char *)&pCons,
			(char *)&Cons,	
			sizeof(HandControls));			
	memcpy(	(char *)&pMeas, 	
			(char *)&Meas, 	
			sizeof(HandMeas));
	memcpy(	&lc,&now,sizeof(now));	
}


double BH280::diffclock(timeval* currentTime, timeval* startTime)
{
	double t2=(double)currentTime->tv_sec+(double)currentTime->tv_usec/1000000.0;
	double t1=(double)startTime  ->tv_sec+(double)startTime  ->tv_usec/1000000.0;
	if (t2-t1 > 0.0)
		return t2-t1;
	else
		return 0.0;
}

void BH280::StopVelocityLoop()
{
	bh.RTAbort();
	result = bh.Command("t");
	shouldRun1=false;
}
void BH280::StopTorqueLoop()
{
	bh.RTAbort();
	result = bh.Command("t");
	shouldRun2=false;
}