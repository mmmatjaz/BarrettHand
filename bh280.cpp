#define POS_SCALE_280_S 1.0
#define POS_SCALE_280_G 1.0
#define TOR_SCALE_280_S 1.0
#define TOR_SCALE_280_G 1.0

#define RANGE_280_S 35840
#define RANGE_280_G 199000

#define Pg 0.01
#define Ps 0.001
#define Dg 0//.001
#define Ds 0//.001
#define MAX_G 0.001
#define MAX_S 0.001

#include "bh280.h"

BH280::BH280()
{
	float const Pi=4.0*atan(1.0);
	props.range[0]=1.0*RANGE_280_G;
	props.range[1]=1.0*RANGE_280_G;
	props.range[2]=1.0*RANGE_280_G;
	props.range[3]=1.0*RANGE_280_S;
	
	props.scaleIN[0]=1.0*RANGE_280_G/(0.7778*Pi);
	props.scaleIN[1]=1.0*RANGE_280_G/(0.7778*Pi);
	props.scaleIN[2]=1.0*RANGE_280_G/(0.7778*Pi);
	props.scaleIN[3]=1.0*RANGE_280_S/(Pi);
	
	props.scaleOUT[0]=1.0*RANGE_280_G/(0.7778*Pi);
	props.scaleOUT[1]=1.0*RANGE_280_G/(0.7778*Pi);
	props.scaleOUT[2]=1.0*RANGE_280_G/(0.7778*Pi);
	props.scaleOUT[3]=1.0*RANGE_280_S/(Pi);
	
	reg.P[0]=Pg; reg.P[1]=Pg; reg.P[2]=Pg; reg.P[3]=Ps;
	reg.D[0]=Dg; reg.D[1]=Dg; reg.D[2]=Dg; reg.D[3]=Ds;
	reg.maxV[0]=MAX_G; reg.maxV[1]=MAX_G; reg.maxV[2]=MAX_G; reg.maxV[3]=MAX_S;
	
	bzero(&din,sizeof(din)); bzero(&dout,sizeof(dout));
	conn=false;conn_=false;
}

void BH280::Error()
{
	sprintf(bufy,"ERROR: %d\n%s\n", result, bh.ErrorMessage(result));
	cout<<bufy<<endl;
	//exit(0);
}

void BH280::Initialize(	bool PPS,
						struct Controls * DIN, struct Measurements * DOUT,
						pthread_mutex_t * mutex,
						timeval * LastReceived)
{
//	copy parameters
	pps=PPS; din_=DIN; dout_=DOUT;
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

double BH280::PositionControl(int m)
{
	// r = position, u = velocity
  double out=1.0;
  out=reg.P[m]* (din.cValues280[m]-dout.m280[0][m]);
  
  if (out>reg.maxV[m]) 		out=reg.maxV[m];
  if (out<-reg.maxV[m]) 	out=-reg.maxV[m];
  return out;
}
double BH280::VelocityControl(int m)
{
	// r = velocity, u = velocity (speed limits only)
  double out=1.0;
  out=din.cValues280[m];
  
  if (out>reg.maxV[m]) 		out=reg.maxV[m];
  if (out<-reg.maxV[m]) 	out=-reg.maxV[m];
  return out;
}
double BH280::TorqueControl(int m)
{
	// r = position, u = torque (speed limits only)
  double out=1.0;
  out=din.cValues280[m];
  
  if (out>reg.maxV[m]) 		out=reg.maxV[m];
  if (out<-reg.maxV[m]) 	out=-reg.maxV[m];
  return out;
}

void BH280::RunRealTime()
{
	int temp;
	
	for (int m = 0; m < 4; m++)//! 3->4
	{	
		dout.m280[0][m] = bh.RTGetPosition(m + '1')/props.scaleOUT[m];
		dout.m280[1][m] = bh.RTGetVelocity(m + '1')/props.scaleOUT[m];
		dout.m280[2][m] = bh.RTGetDeltaPos(m + '1')/props.scaleOUT[m];
		dout.m280[3][m] = bh.RTGetStrain(m + '1');
		if (pps)
			bh.RTGetPPS(m + '1', &dout.pps[m][0], MAX_PPS_ELEMENTS);	
	}
	
	// read & write server buffers
	pthread_mutex_lock( Mutex );		
	
		memcpy(	(char *)&(din.Mode280),
				(char *)&(din_->Mode280),	
				sizeof(Controls)/2);
		memcpy(	(char *)dout_->m280, 	
				(char *)dout.m280, 	
				sizeof(dout.m280));
		if (pps)
		 memcpy((char *)dout_->pps,
				(char *)dout.pps,
				sizeof(dout.pps));	
		
	pthread_mutex_unlock( Mutex );

	int ctrl=(int)(din.Mode280+0.5);
	
	
	// set data to hand
	switch (ctrl)
	{
	case 1:	// velocity control		
		for (int m = 2; m < 3; m++)
		{
			temp=(int)((VelocityControl(m) * props.scaleIN[m])+0.5);
			result=bh.RTSetVelocity(m + '1', temp);
		}
		
		break;
	case 2:	// position control
		for (int m = 2; m < 3; m++)
		{	
			temp=(int)((PositionControl(m) * props.scaleIN[m])+0.5);
			//cout<<"\n"<<temp;
			result=bh.RTSetVelocity(m + '1', temp);
		}
		break;

	case 3:	// torque control
		for (int m = 2; m < 3; m++)
		{	
			//temp=(int)((TorqueControl(m) * props.scaleIN[m])+0.5);
			cout<<"\n"<<(int)(din.cValues280[m]);
			// result=bh.RTSetTorque('3', 50);
			 //result=bh.RTSetTorque(m + '1', (int)(din.cValues280[m]));
		}
		break;
	}
	
	bh.RTUpdate();
	//gettimeofday(&read2, NULL);
	
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
		//cout<<conn<<" "<<conn_<<endl;
		if (!conn_ && !conn) 
		{
			usleep(1000);// do nothing ;//printf("\rnothing");
		}
		if (!conn_ && conn) // begin
		{
			cout<<"\nStarting\n";//cout<<"\nClient connecting";
			if(PrepareRealTime() )// && !bhRTrunning)
				cout<<"Fail"<<endl;
		}
		if (conn_ && conn) // normal
		{		
			RunRealTime();	
		}
		if (conn_ && !conn)
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

int BH280::PrepareRealTime()
{
	cout<<("PrepareRealTime()....");
	char reply[50];
	int  ctrl=0;
	while(ctrl<1 || ctrl>3) 
	{
		usleep(1000);
		pthread_mutex_lock( Mutex );		
		memcpy(	&(din),
				(din_),	
				sizeof(Controls));
		pthread_mutex_unlock( Mutex );
		ctrl=(int)((din.Mode280)+0.5);
	}
	//printf("mode @%i %f = %f : %f \n", &din_->Mode280, din.Mode280,din_->Mode280,din.cValues280[0]);
	
	cout<<"mode"<<ctrl<<" ";
	switch(ctrl)
	{
		case 1:	// velocity control
			cout<<"Velocity mode ";
			//if(result = bh.Set("","LCP",0)) ;//Error();
			if (result = bh.RTSetFlags( "1234", 1, 3, 0, 0, 1, 1, 1, 1, 1, 1,0,0,0,0 ))
				;//Error();
			break;

		case 2:	// position control
		  cout<<"Position mode ";
		  //if(result = bh.Set("","LCP",0)) Error();
			if (result = bh.RTSetFlags( "1234", 1, 3, 0, 0, 1, 1, 1, 1, 1, 1,0,0,0,0 ))
				Error();
			break;
		
		case 3:	// torque control
			cout<<"Torque mode ";			
			if (result = bh.RTSetFlags( "1234", 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,0,0,0,0 ))
				Error();
			break;
	}
	
	cout<<("RTStart()....");

	if (result = bh.RTStart( "1234" )) ;//Error();
	else cout<<("OK\n");

	bh.RTUpdate();
	bh.Command("1fset tstop 10000");
	bh.Command("2fset tstop 10000");
	bh.Command("3fset tstop 10000");
	bh.Command("sset tstop 10000");
	bh.Command("3fsave");

	return 0;
}
void BH280::Stop()
{
	bh.RTAbort();
	result = bh.Command("t");
	shouldRun=false;
}
double BH280::diffclock(timeval* currentTime, timeval* startTime)
{
	double t2=(double)currentTime->tv_sec+(double)currentTime->tv_usec/1000000.0;
	double t1=(double)startTime->tv_sec+(double)startTime->tv_usec/1000000.0;
	if (t2-t1 > 0.0)
	{
		return t2-t1;
	}
	else
	{
		return 1.0;
	}
}