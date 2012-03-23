#ifndef APP_H
#define APP_H

#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <deque>
#include <time.h>// Needed for ms time.
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/time.h>// Needed for ms time.
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>
#define threadMethod void*  
#endif

#ifdef _WIN32 || defined _WIN64

#include <iostream>
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <deque>
#include <time.h>
#include <conio.h>
#include <process.h>
#include <windows.h>

#include "..\port2win\timeofday.h"
#include "..\port2win\winpthreads.h"

typedef void  threadMethod;
//#define threadMethod void*  
#endif

using namespace std;


struct HandControls
{
	double Mode;
	double cValues[4];
};

struct Command
{
	char command262[4*8];
	char command280[4*8];
};

struct Reply
{
	bool executed262;
	bool executed280;
	char reply262[10*8];
	char reply280[10*8];
};

class App
{
public:
	//App();
	//~App();

	void PrintBanner();
	double DiffClock(timeval* currentTime, timeval* startTime);
};


// CONFIG CLASS
class Config
{
private:
	void Usage();
public:
	int port;
	bool use280;
	bool use262;
	bool usePPS;
	int serialPort;
	
	double 	ST;
	Config();
	void FromArgs(int argc, char* argv[]);
	void PrintConf();
};


#endif