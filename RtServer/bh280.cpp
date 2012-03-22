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
#define MAX_G 0.003
#define MAX_S 0.001

#define VEL_CONTROL 1
#define POS_CONTROL 2
#define TOR_CONTROL 3

#define NO_CONN		!conn_ && !conn
#define CONN		 conn_ &&  conn
#define CONN_LOST	 conn_ && !conn
#define CONN_INIT	!conn_ &&  conn

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
	
	props.scaleOUT[0]=1.0*RANGE_280_G/(0.7778*PI);
	props.scaleOUT[1]=1.0*RANGE_280_G/(0.7778*PI);
	props.scaleOUT[2]=1.0*RANGE_280_G/(0.7778*PI);
	props.scaleOUT[3]=1.0*RANGE_280_S/(PI);
	
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
	Mutex=mutex; tstamp=LastReceived; shouldRun=true;

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



double BH280::PositionControlC(int m)
{
	LowPass(m,0.2);
  double out=0.0;
  double dt=diffclock(&now,&lc);
  double Kf=0.5/10.0;
  //P
	double qd=Cons.cValues[m];
	double qf=Kf*Deadzone(m,0.30);
	double q= Meas.Position[m];
	double e= qd-q-qf;
	//Meas.m280[2][m]=qd-q-qf;
  //D
	double pqd=pCons.cValues[m];
	double pqf=Kf*SmoothDeadzone(pMeas.StrainFilt[m]);
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

double BH280::LowPassAny(double x, double y_, double a)
{
  double dt=diffclock(&now,&lc);
  double Y=x*dt/(a+dt)+y_*a/(a+dt);
  return Y;
}

void BH280::LowPass2(int m,double aa)
{
/*
	double dt1=diffclock(&now,&lc);
	double dt2=sCache[m].dt;
	double x=(Meas.Strain[m]-126);
	double y_1=sCache[m].y_1;
	double y_2=sCache[m].y_2;
	double a=100.0;
	double K=a*a;
	double y_0=(dt1*y_1 - dt1*y_2 + dt2*y_1 + 2*dt1*dt2*y_1 + K*dt1*dt1*dt2*x)
			/(dt2*a*a*dt1*dt1 + 2*dt2*dt1 + dt2);
	sCache[m].y_2=sCache[m].y_1;
	sCache[m].y_1=sCache[m].y_0;
	sCache[m].dt=dt1;
	Meas.StrainFilt[m]=y_0;
	cout << "dt1 " << dt1 <<"dt2 "<<dt2<<" s: "<<x<<" sf: "<<y_0<<endl;
*/
}

double BH280::SmoothDeadzone(double value)
{
	double tresh=7;
	double level=(value/tresh)*(value/tresh);
	if (level<-1) level=-1;
	if (level> 1) level= 1;
	return level*value;
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

void BH280::RunRealTime()
{
	ReadFromHand();
	RefreshData();
	SendToHand();
	RememberData();
}

void BH280::Loop()
{
/*  *****************************
			hand thread
	***************************** */
	timeval time2;
	double T;
	while(shouldRun)
	{	
		gettimeofday(&time2, NULL);
		pthread_mutex_lock( Mutex );
		T=diffclock(&time2,tstamp);
		pthread_mutex_unlock( Mutex );
	
		if (T<0.3) conn=true; 	
		else conn=false;
		
		if ( NO_CONN ) 
		{
			usleep(1000);
		}
		if ( CONN_INIT ) 
		{
			Begin();
		}
		if ( CONN && realtime) // normal
		{		
			RunRealTime();	
		}
		if ( CONN_LOST && realtime)
		{
			cout<<"\nDisconnecting\n"<<endl;
			bh.RTAbort();
			result = bh.Command("t");
		}
		
		conn_=conn;
	
	}
	bh.RTAbort();
	result = bh.Command("t");
	cout<<"Terminating thread bh280\n";
}

void BH280::LoopOffline()
{
/*  *****************************
			hand thread
	***************************** */
	timeval Tstart;
	timeval Tnow;
	timeval Tbefore;
	double T;
	
	if (result = bh.Command("GFSET DP 100000"))
		Error();

	if (result = bh.Command("123m"))
		Error();
		
	if (result = bh.RTSetFlags( "1234", 1, 3, 0, 0, 1, 1, 1, 1, 1, 1,0,0,0,0 ))
		Error();
	if (result = bh.RTStart( "1234" )) 
		Error();
	bh.RTUpdate();
	T=0.0;
	gettimeofday(&lc, NULL);
	while(shouldRun)
	{	
		gettimeofday(&now, NULL);
		T+=diffclock(&now,&lc);
		ReadFromHand();
		RefreshMeas();
		double f=0.2;//Hz
		int m=1;
		double value = PI/3 + (PI/4*sin( f* T*2*PI));
		Cons.cValues[m]=value;		
		
		int temp=(int)((PositionControlC(m) * props.scaleIN[m])+0.5);
		result=bh.RTSetVelocity(m + '1', temp);
		
		bh.RTUpdate();
		
		printf("\ntime: %4.2f r: %4.2f y: %4.2f f: %4.2f",T,value,Meas.Position[m],pMeas.StrainFilt[m]);
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
	timeval Tstart;
	timeval Tnow;
	timeval Tbefore;
	double T;
	
	if (result = bh.RTSetFlags( "1234", 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,0,0,0,0 ))
		Error();
	
	if (result = result = bh.Set("1234", "LCP", 1))
		Error();
	if (result = result = bh.Set("1234", "LCV", 0))
		Error();
	if (result = result = bh.Set("1234", "LCT", 1))
		Error();
	
	if (result = bh.RTStart( "1234" )) 
		Error();
	bh.RTUpdate();
	T=0.0;
	gettimeofday(&lc, NULL);
	while(shouldRun)
	{	
		gettimeofday(&now, NULL);
		T+=diffclock(&now,&lc);
		ReadFromHand();
		RefreshMeas();
		int temp=0;
		int m=1;
		while(Meas.Position[m]<0.1)
		{
			if (result=bh.RTSetTorque(m + '1', temp))
				Error();
			temp++;
			printf("\n%i",temp);
		}
		result=bh.RTSetTorque(m + '1',0);
		cout<<"done at "<<temp<<endl;
			
		bh.RTUpdate();
		
		RememberData();
	}
	bh.RTAbort();
	result = bh.Command("t");
	cout<<"Terminating thread bh280\n";
}

int BH280::Begin()
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
	
	switch(ctrl)
	{
		case VEL_CONTROL:	// velocity control
			cout<<"PrepareRealTime()... Velocity mode "<<endl;
			if (result = bh.RTSetFlags( "1234", 1, 3, 0, 0, 1, 1, 1, 1, 1, 1,0,0,0,0 ))
				Error();
			realtime=true;
			break;

		case POS_CONTROL:	// position control
		  cout<<"PrepareRealTime()... Position mode "<<endl;
			if (result = bh.RTSetFlags( "1234", 1, 3, 0, 0, 1, 1, 1, 1, 1, 1,0,0,0,0 ))
				Error();
			realtime=true;
			break;
		
		case TOR_CONTROL:	// torque control
			cout<<"PrepareRealTime()... Torque mode "<<endl;					
			if (result = bh.RTSetFlags( "1234", 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,0,0,0,0 ))
				Error();
				/*
			if(result = bh.Set("4","LCP",1))
				;*/
			realtime=true;
			break;		
	}
	
	cout<<("RTStart()....");

	if (result = bh.RTStart( "1234" )) ;//Error();
	else cout<<("OK\n");

	bh.RTUpdate();
	bh.Command("1fset tstop 10000");bh.Command("2fset tstop 10000");
	bh.Command("3fset tstop 10000");bh.Command("sset tstop 10000");
	bh.Command("3fsave");
	return 0;
}

void BH280::Stop()
{
	bh.RTAbort();
	result = bh.Command("t");
	shouldRun=false;
}
void BH280::ReadFromHand()
{
	for (int m = 0; m < 4; m++)//! 3->4
	{	
		Meas.Position[m] = bh.RTGetPosition(m + '1')/props.scaleOUT[m];
		Meas.Velocity[m] = bh.RTGetVelocity(m + '1')/props.scaleOUT[m];
		Meas.DeltaPos[m] = bh.RTGetDeltaPos(m + '1')/props.scaleOUT[m];
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
void BH280::RefreshMeas()
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
void BH280::SendToHand()
{
	gettimeofday(&now, NULL);
	int temp;
	int ctrl=(int)(Cons.Mode+0.5);
	// set data to hand
	switch (ctrl)
	{
	case 1:	// velocity control		
		for (int m = 0; m < 3; m++)
		{
			temp=(int)((VelocityControl(m) * props.scaleIN[m])+0.5);
			//PositionControlC(m);
			result=bh.RTSetVelocity(m + '1', temp);
		}
		break;
	case 2:	// position control
		for (int m = 0; m < 3; m++)
		{	
			temp=(int)((PositionControlC(m) * props.scaleIN[m])+0.5);
			//cout<<"\n"<<temp;
			result=bh.RTSetVelocity(m + '1', temp);
		}
		break;

	case 4:	// torque control
		for (int m = 3; m < 4; m++)
		{	
			//temp=(int)((TorqueControl(m) * props.scaleIN[m])+0.5);
			cout<<"\r"<<temp;
			if (result = bh.RTSetTorque(m + '1', temp))
				Error();			
		}
		break;
		
	case 3:	// zero force control
		for (int m = 2; m < 3; m++)
		{	
			//temp=(int)((ForceControl(m)* props.scaleIN[m])+0.5);
			cout<<"\n"<<temp;		
			result=bh.RTSetVelocity(m + '1', temp);			
		}
		break;
	
	}
	bh.RTUpdate();
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