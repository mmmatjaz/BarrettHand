#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <deque>
#include <sys/time.h>// Needed for ms time.
#include <time.h>// Needed for ms time.
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

struct Controls
{
	double Mode262;
	double cValues262[4];
	double Mode280;
	double cValues280[4];
};
struct Measurements
{
	double time;
	double m262[4][4];
	double m280[4][4];
	int pps[4][24];
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
	int RXport;
	int TXport;
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