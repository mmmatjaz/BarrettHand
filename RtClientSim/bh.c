/*
 * Use compiler :
 *    Lcc-win32 C 2.4.1
 *  Compile with :
 *    mex  bh.c C:\Pr'ogram Files (x86)'\MATLAB'\R2009b'\sys'\lcc'\lib'\wsock32.lib' -DWIN32
 *
 *
 * Use compiler :
 *    Microsoft Visual C++ 2008 SP1  
 *  Compile with : *
 *      mex bh.c C:\P'rogram Files'\M'icrosoft SDKs'\Windows'\v6.0A'\Lib'\WSock32.Lib' -DWIN32
 */

#define S_FUNCTION_NAME  bh
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"


/******* GENERAL DEFINES *********/
#include        <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include "mex.h"   //--This one is required
#include "matrix.h"
#include <string.h>
//#include        <windows.h>
/******* WINDOWS ONLY DEFINES *********/
#ifdef WIN32
#define IFWINDOWS(dothis) dothis 
#define IFUNIX(dothis)

#include <winsock2.h>

#define close(s) closesocket(s)
#define nonblockingsocket(s) {unsigned long ctl = 1;ioctlsocket( s, FIONBIO, &ctl );}
#define s_errno WSAGetLastError()
#define EWOULDBLOCK WSAEWOULDBLOCK
#define usleep(a) Sleep((a)/1000)
#define MSG_NOSIGNAL 0

#endif
#define PORT 4444
#define IP "193.2.6.168"

#define OUT_1_W		1 //time
#define OUT_2_W		4 //pos
#define OUT_3_W		4 //vel
#define OUT_4_W		4 //deltap
#define OUT_5_W		4 //strain
#define OUT_6_W		4 //strainFilt
#define OUT_7_W		4
#define OUT_8_W		4
#define OUT_9_W		4
#define OUT_10_W	4
#define OUT_11_W	4
#define OUT_12_W	4*24


#define NUMBER_OF_ARGS          (17)
#define IP_ADDRESS_ARG          ssGetSFcnParam(S,0)
#define IP_PORT_ARG           	ssGetSFcnParam(S,1) //receive port
#define MODE_1		           	ssGetSFcnParam(S,2)
#define MODE_2		          	ssGetSFcnParam(S,3)
#define ST						ssGetSFcnParam(S,4)
#define OUT_1		          	ssGetSFcnParam(S,5)
#define OUT_2		          	ssGetSFcnParam(S,6)
#define OUT_3		          	ssGetSFcnParam(S,7)
#define OUT_4		          	ssGetSFcnParam(S,8)
#define OUT_5		          	ssGetSFcnParam(S,9)
#define OUT_6		          	ssGetSFcnParam(S,10)
#define OUT_7		          	ssGetSFcnParam(S,11)
#define OUT_8		          	ssGetSFcnParam(S,12)
#define OUT_9		          	ssGetSFcnParam(S,13)
#define OUT_10		          	ssGetSFcnParam(S,14)
#define OUT_11		          	ssGetSFcnParam(S,15)
#define OUT_12		          	ssGetSFcnParam(S,16)

#define MODE_1_             ((int_T)  mxGetPr(MODE_1)[0])
#define MODE_2_	         	((int_T)  mxGetPr(MODE_2)[0])
#define ST_					((int_T)  mxGetPr(ST)[0])
#define OUT_1_            	((int_T)  mxGetPr(OUT_1)[0])
#define OUT_2_       		((int_T)  mxGetPr(OUT_2)[0])
#define OUT_3_         		((int_T)  mxGetPr(OUT_3)[0])
#define OUT_4_      		((int_T)  mxGetPr(OUT_4)[0])
#define OUT_5_       		((int_T)  mxGetPr(OUT_5)[0])
#define OUT_6_       		((int_T)  mxGetPr(OUT_6)[0])
#define OUT_7_     			((int_T)  mxGetPr(OUT_7)[0])
#define OUT_8_        		((int_T)  mxGetPr(OUT_8)[0])
#define OUT_9_       		((int_T)  mxGetPr(OUT_9)[0])
#define OUT_10_     		((int_T)  mxGetPr(OUT_10)[0])
#define OUT_11_     		((int_T)  mxGetPr(OUT_11)[0])
#define OUT_12_     		((int_T)  mxGetPr(OUT_12)[0])

struct HandMeas
{
	double Position[4];
	double Velocity[4];
	double DeltaPos[4];
	double Strain[4];
	double StrainFilt[4];
};
struct HandMeasPPS
{
	double Position[4];
	double Velocity[4];
	double DeltaPos[4];
	double Strain[4];
	double StrainFilt[4];
	int pps[4][24];
};
struct HandControls
{
	double Mode;
	double cValues[4];
};

struct Controls
{
	struct HandControls con262;
	struct HandControls con280;
};

struct Measurements
{
	double time;
	struct HandMeas 	hdata262;
	struct HandMeasPPS  hdata280;
};


int_T mode1;
int_T mode2;
double sampleTime;
struct Controls cons;
struct Measurements meas;

  struct sockaddr_in server;
  struct sockaddr_in client;
  WSADATA wsaData;
  SOCKET sockSerRem;
  
  int nTimeout = 100; // 0.1 seconds delay for nonblocking socket  
  
  int i,j,k; 
  int server_length=sizeof(struct sockaddr_in);
  
  static char_T errMsg2[256];
  
//---------------------------------------------------------------------------------------------------
int SendCommand()
//---------------------------------------------------------------------------------------------------
{
    int n = 0;
    //send the commands structure
	n = sendto(sockSerRem, (void*)&cons, sizeof(cons), 0, (struct sockaddr*) &server,server_length );
//     printf("Sent packet %d.\n", n);
	if(n==-1)
    {
      fprintf(stderr, "Error transmitting data(sending command).\n");
    }
	return n;
}

//---------------------------------------------------------------------------------------------------
int ReceiveMeasurement()
//---------------------------------------------------------------------------------------------------
{
    //updates the measurements structure
    int i = 0;
    int n = 0;
    const int TIMEOUT = 100;
 //   while ((n < 10) && (i < TIMEOUT))
  //  {
        n = recvfrom(sockSerRem, (char*) &meas, sizeof(meas), 0, (struct sockaddr*)&server, &server_length);
        i++;
        Sleep(1);
   // }
   
    if (i >= TIMEOUT)
    {
         printf("# ERROR: no telegram (Receive).\n");
    }
    
   // printf("Received packet %d., Buffer: %d\n", n,sizeof(meas));
	return n;
}
//---------------------------------------------------------------------------------------------------
void ClearReceiveBuffer()
//---------------------------------------------------------------------------------------------------
{
    /*
	//clears the receive buffer
    int n = 1;

    while (n > 0)
    {
        n = recvfrom(sockSerRem, (char*) &measurements, sizeof(measurements), 0, (struct sockaddr*)&server, &server_length);
    }
	*/
}

//---------------------------------------------------------------------------------------------------


static void mdlInitializeSizes(SimStruct *S)
{    
	int output_size=0;
    int index=0;
	ssSetNumSFcnParams(    S, NUMBER_OF_ARGS);  
	
	ssSetNumSampleTimes(S, 1);
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
	
    // Specify I/O
    if (!ssSetNumInputPorts(S, 2)) return;
    ssSetInputPortWidth(S, 0, 4);
    ssSetInputPortWidth(S, 1, 4);
	
	ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
	
    output_size = OUT_1_+OUT_2_+OUT_3_+OUT_4_+OUT_5_+OUT_6_+OUT_7_+OUT_8_+OUT_9_+OUT_10_+OUT_11_+OUT_12_;
	printf("\n%i\n",output_size);
    if (!ssSetNumOutputPorts(S,output_size)) return;
		
    if (OUT_1_==1) {ssSetOutputPortWidth(S, index, 1);index++;}
    if (OUT_2_==1) {ssSetOutputPortWidth(S, index, 4);index++;}
    if (OUT_3_==1) {ssSetOutputPortWidth(S, index, 4);index++;}
	if (OUT_4_==1) {ssSetOutputPortWidth(S, index, 4);index++;}
	if (OUT_5_==1) {ssSetOutputPortWidth(S, index, 4);index++;}
	if (OUT_6_==1) {ssSetOutputPortWidth(S, index, 4);index++;}
	if (OUT_7_==1) {ssSetOutputPortWidth(S, index, 4);index++;}
	if (OUT_8_==1) {ssSetOutputPortWidth(S, index, 4);index++;}
	if (OUT_9_==1) {ssSetOutputPortWidth(S, index, 4);index++;}
	if (OUT_10_==1){ssSetOutputPortWidth(S, index, 4);index++;}
	if (OUT_11_==1){ssSetOutputPortWidth(S, index, 4);index++;}
    if (OUT_12_==1){ssSetOutputPortWidth(S, index,96);index++;}
	
    mode1= 		(int_T)mxGetPr(MODE_1)[0];
	mode2= 		(int_T)mxGetPr(MODE_2)[0];	
    //printf("\nInit Sizes: mode: %i %i\n",mode1,mode2);
	//'193.2.6.168' 4444 1 1 0.005 1 1 1 1 1 1 1 1 1 1 1
	//'193.2.6.168' 4444 0 0 0.005 0 0 0 0 0 0 0 0 0 0 0
	printf("mdlInitializeSizes\n");
}



static void mdlInitializeSampleTimes(SimStruct *S)
{
	sampleTime = (double)mxGetPr(ST)[0];
    ssSetSampleTime(S, 0, sampleTime);
}



#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)

  static void mdlInitializeConditions(SimStruct *S)
  {
  }
#endif /* MDL_INITIALIZE_CONDITIONS */

        

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
 
static void mdlStart(SimStruct *S)
{   
	int TIMEOUT_start = 4000;
	int j=0;
	int_T           Port;
	
	uchar_T     ipAddress[16];
	Port= 		(int_T)mxGetPr(IP_PORT_ARG)[0];
	//mode1= 		(int_T)mxGetPr(MODE_1)[0];
	//mode2= 		(int_T)mxGetPr(MODE_2)[0];
	mxGetString(IP_ADDRESS_ARG, ipAddress, 16);
	
	printf("\nmode %i %i\n",mode1,mode2);
	
      //!Open UDP connection to server
	WSAStartup(MAKEWORD(2,2), &wsaData);

	sockSerRem = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(sockSerRem, SOL_SOCKET, SO_REUSEADDR, 0, 0);  
	setsockopt(sockSerRem, SOL_SOCKET, SO_RCVTIMEO, (const char*)&nTimeout, sizeof(int)); 
	setsockopt(sockSerRem, SOL_SOCKET, SO_SNDTIMEO, (const char*)&nTimeout, sizeof(int));

	if( sockSerRem==INVALID_SOCKET)
	{
		fprintf(stderr,"Could not create socket.\n");
		closesocket(sockSerRem);
		WSACleanup();
		mexErrMsgTxt("Could not create socket.");
	}

	memset((char *) &server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(Port);
	//       server.sin_port = htons(SERVER_PORT_B);

	server.sin_addr.s_addr = inet_addr(ipAddress);

	memset((char *) &client, 0, sizeof(struct sockaddr_in));
	client.sin_family = AF_INET;
	client.sin_port = htons(0);
	client.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockSerRem,(struct sockaddr*)&client,sizeof(struct sockaddr_in))==-1)
	{
		fprintf(stderr,"Cannot bind address to socket.\n");
		closesocket(sockSerRem);
		WSACleanup();
		mexErrMsgTxt("Cannot bind address to socket.");
	}

	ClearReceiveBuffer();
   
     memset(&cons,0,sizeof(cons)); memset(&meas,0,sizeof(meas));
    if(SendCommand() < 0)
    {
        mexErrMsgTxt("Error transmitting data to Kuka server.");  
    }
	
  
 }
#endif /*  MDL_START */


static void mdlOutputs(SimStruct *S, int_T tid)
{
}

#define MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
 
  static void mdlUpdate(SimStruct *S, int_T tid)
  { 
    InputRealPtrsType  u1 = ssGetInputPortRealSignalPtrs(S,0);
    InputRealPtrsType  u2 = ssGetInputPortRealSignalPtrs(S,1);
	
	real_T *y0;
	real_T *y1;
	real_T *y2;
	real_T *y3;
	real_T *y4;
	real_T *y5;
	real_T *y6;
	real_T *y7;
	real_T *y8;
	real_T *y9;
	real_T *y10;
	real_T *y11;
	
	int temp[24*4];
	int z;
	int index;
	z=0;
	index=0;
    if (OUT_1_==1) {y0  = ssGetOutputPortRealSignal(S, index);index++;}	
	if (OUT_2_==1) {y1  = ssGetOutputPortRealSignal(S, index);index++;}
	if (OUT_3_==1) {y2  = ssGetOutputPortRealSignal(S, index);index++;}
	if (OUT_4_==1) {y3  = ssGetOutputPortRealSignal(S, index);index++;}
    if (OUT_5_==1) {y4  = ssGetOutputPortRealSignal(S, index);index++;}
    if (OUT_6_==1) {y5  = ssGetOutputPortRealSignal(S, index);index++;}
    if (OUT_7_==1) {y6  = ssGetOutputPortRealSignal(S, index);index++;}
    if (OUT_8_==1) {y7  = ssGetOutputPortRealSignal(S, index);index++;}
    if (OUT_9_==1) {y8  = ssGetOutputPortRealSignal(S, index);index++;}
    if (OUT_10_==1){y9  = ssGetOutputPortRealSignal(S, index);index++;}
    if (OUT_11_==1){y10 = ssGetOutputPortRealSignal(S, index);index++;}
    if (OUT_12_==1){y11 = ssGetOutputPortRealSignal(S, index);index++;}
	
	cons.con262.Mode=(double)mode1; //control mode bh262
    cons.con280.Mode=(double)mode2; //control mode bh280
	//printf("mode %f %f\n",cons.mode262,cons.mode280);
	
	memcpy(cons.con262.cValues,u1[0],4*sizeof(double)); // control values
    memcpy(cons.con280.cValues,u2[0],4*sizeof(double));
    
    if(SendCommand() < 0)
    {
        mexErrMsgTxt("Error transmitting data to Kuka server.");
       
    }
    
    if(ReceiveMeasurement() < 0) 
    {
        mexErrMsgTxt("Error receiving data from Kuka server.");      
    }  
	
	if (OUT_1_==1) y0[0] = meas.time;
	for(z=0;z<4;z++)
	{
		if (OUT_2_==1)   y1[z]=meas.hdata262.Position[z];
		if (OUT_3_==1)   y2[z]=meas.hdata262.Velocity[z];
		if (OUT_4_==1)   y3[z]=meas.hdata262.DeltaPos[z];
		if (OUT_5_==1)   y4[z]=meas.hdata262.Strain[z];
		if (OUT_6_==1)   y5[z]=meas.hdata262.StrainFilt[z];
		if (OUT_7_==1)   y6[z]=meas.hdata280.Position[z];
		if (OUT_8_==1)   y7[z]=meas.hdata280.Velocity[z];
		if (OUT_9_==1)   y8[z]=meas.hdata280.DeltaPos[z];
		if (OUT_10_==1)  y9[z]=meas.hdata280.Strain[z];
		if (OUT_11_==1) y10[z]=meas.hdata280.StrainFilt[z];
	}
	
	if (OUT_12_==1)
	{
		memcpy(temp,meas.hdata280.pps,24*4*4);
		for (z=0;z<24*4;z++)
			y11[z]=(double)temp[z];
	}
    printf("\n%f %f %f %f",cons.con280.cValues[0],cons.con280.cValues[1],
								cons.con280.cValues[2],cons.con280.cValues[3]);
    
	
  }
#endif /* MDL_UPDATE */



#define MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  /* Function: mdlDerivatives =================================================
   * Abstract:
   *    In this function, you compute the S-function block's derivatives.
   *    The derivatives are placed in the derivative vector, ssGetdX(S).
   */
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */


static void mdlTerminate(SimStruct *S)
{
    closesocket(sockSerRem);
    WSACleanup();
    printf("Socket closed (mdl_terminate).\n");
}



#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
