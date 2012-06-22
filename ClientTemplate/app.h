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
#endif

using namespace std;

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
	HandControls con262;
	HandControls con280;
};

struct Measurements
{
	double 		time;
	HandMeas 	hdata262;
	HandMeasPPS hdata280;
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