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

#include "graphics.h"
using namespace std;

#define WINPOSX 500
#define WINPOSY 500
#define WINSIZEX 500
#define WINSIZEY 400
#define INT_GLUT_BITMAP_HELVETICA_10  5
#define INT_GLUT_BITMAP_HELVETICA_12  6
#define INT_GLUT_BITMAP_HELVETICA_18  7

int glutWin;
HandMeasPPS pMeas;
	HandMeasPPS * Meas_;
	pthread_mutex_t * Mutex;
	bool shouldRun=true;

void processNormalKeys(unsigned char key, int x, int y) 
{
	glutDestroyWindow(glutWin);
}

void changeSize(int w, int h) 
{
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
}

void setColorPPS(int index)
{
	float color=(float)(index)/2000;//+0.5;
	glColor3f(0,color+0.1,color+0.3);
}

void setColorStrain(float index)
{
	float color=index/255;//+0.25;
	glColor3f(0.8,0.1+0.8*color,0);
}

void renderBitmapString(float x,float y,float z,void *font,char *string) 
{
	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(font, *c);
	}
}


void renderScene(void) 
{
	float xx=0.3;
	float yy=0.3;
	float dx=0.05;
	float dy=0.05;
	float x0, y0;
	float x1,x2,y1,y2;
	float d1, d2;
	float Rx, Ry;
	float clr=1;
	float xModel, yModel;

	int send280pps_[4][24];
	float send280_[4][4];
	float tempStrain[4];
	int dp=0;//counter kvadratkov
	float color;
	double const Pi=4*atan(1.0);
	float angle=0.0;
	//Sleep(20);
	
	pthread_mutex_lock( Mutex );
	
		for (int i=0;i<4;i++)
		{
			tempStrain[i]=(float)Meas_->Strain[i];
			send280_[0][i]=(float)Meas_->Position[i];
		}
		memcpy(send280pps_,	Meas_->pps,	sizeof(send280pps_));
		angle=(float)Meas_->Position[3];
	pthread_mutex_unlock( Mutex );
	
	angle=send280_[0][3];
	//printf("\n%f",angle);
	for (int i=0;i<4;i++)
	{
		tempStrain[i]=(float)send280_[3][i];
	}

	//angle = -1.0*send280_[3][0];//*180/Pi+180);
	angle*=-180/Pi;
	angle+=180;
	
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	0.0f, 0.0f, 10.0f,
			0.0f, 0.0f,  0.0f,
			0.0f, 1.0f,  0.0f);

	/////////////////////////////////
	// BACKGROUND
	/////////////////////////////////
	//glColor3f(0.9,0.9,0.9);
	//glRectf(-6.0, -6.0, 6.0, 6.0);

	glBegin(GL_QUADS);
	//red color
	glColor3f(0.6,0.6,0.6);
	glVertex2f(-6.0, 6.0);
	glVertex2f(-6.0,-6.0);
	//blue color
	glColor3f(0.9,0.9,0.9);
	glVertex2f(6.0,-6.0);
	glVertex2f(6.0, 6.0);
	glEnd();
	glLoadIdentity();
	gluLookAt(	0.0f, 0.0f, 10.0f,
			0.0f, 0.0f,  0.0f,
			0.0f, 1.0f,  0.0f);
	/////////////////////////////////
	// PALM
	/////////////////////////////////
	x0=-(7*xx+2*dx)/2;
	y0=-(4*xx+2*dx)/2;
	bool mm = false;
	bool nn = false;
	dp=0;
	for (int m=0; m<4; m++)
	{
		for (int n=0; n<7; n++)
		{
			if (m==0 || m==3) mm=true;
			if (n==0 || n==6) nn=true;
			if (!(mm && nn))
			{
				x1 = n*xx + x0 + dx;
				x2 = (n+1)*xx + x0;

				y1 = m*yy + y0 + dy;
				y2 = (m+1)*yy + y0;

				setColorPPS(send280pps_[3][dp]);
				glRectf(x1, y1, x2, y2);
				dp++;
			}
			mm = false;
			nn = false;
		}
	}
	
	/////////////////////////////////
	// F3
	/////////////////////////////////
	x0=-(3*xx+2*dx)/2;
	y0=1;
	dp=0;
	for (int m=0; m<8; m++)
	{
		for (int n=0; n<3; n++)
		{
			x1 = n*xx + x0 + dx;
			x2 = (n+1)*xx + x0;

			y1 = m*yy + y0 + dy;
			y2 = (m+1)*yy + y0;
			setColorPPS(send280pps_[2][dp]);
			glRectf(x1, y1, x2, y2);	
			dp++;
		}
	}
	//	strain
	x1 = 0*xx + x0 + dx;
	y1 = 8*yy + y0 + dy;
	x2 = (2+1)*xx + x0;
	y2 = (8+0.5+1)*yy + y0;
	
	setColorStrain(0.8*tempStrain[2]);
	glRectf(x1, y1, x2, y2);

	setColorStrain(tempStrain[2]);
	glRectf(x1+dx, y1+dy, x2-dy, y2-dy);
	
	
	/////////////////////////////////
	// F1
	/////////////////////////////////
	glTranslatef(-(3*xx+2*dx)/2-0.5, 0, 0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	
	x0=-(3*xx+2*dx)/2;//-(3*xx+2*dx)/2-1;
	y0=1;//1;
	dp=0;
	for (int m=0; m<8; m++)
	{
		for (int n=0; n<3; n++)
		{
			x1 = n*xx + x0 + dx;
			x2 = (n+1)*xx + x0;

			y1 = m*yy + y0 + dy;
			y2 = (m+1)*yy + y0;
			setColorPPS(send280pps_[0][dp]);
			glRectf(x1, y1, x2, y2);
			dp++;
		}
	}
	//	strain
	x1 = 0*xx + x0 + dx;
	y1 = 8*yy + y0 + dy;
	x2 = (2+1)*xx + x0;
	y2 = (8+0.5+1)*yy + y0;
	/*
	setColorStrain(tempStrain[0]);
	glRectf(x1, y1, x2, y2);
	*/
	setColorStrain(0.8*tempStrain[0]);
	glRectf(x1, y1, x2, y2);

	setColorStrain(tempStrain[0]);
	glRectf(x1+dx, y1+dy, x2-dy, y2-dy);
	/////////////////////////////////
	// F2
	/////////////////////////////////
	
	glEnd();
	glLoadIdentity();
	gluLookAt(	0.0f, 0.0f, 10.0f,
			0.0f, 0.0f,  0.0f,
			0.0f, 1.0f,  0.0f);
			
	glTranslatef((3*xx+2*dx)/2+0.5, 0, 0);
	glRotatef(-angle, 0.0f, 0.0f, 1.0f);
	
	x0=-(3*xx+2*dx)/2;//-(3*xx+2*dx)/2-1;
	y0=1;//1;
	dp=0;
	for (int m=0; m<8; m++)
	{
		for (int n=0; n<3; n++)
		{
			x1 = n*xx + x0 + dx;
			x2 = (n+1)*xx + x0;
			y1 = m*yy + y0 + dy;
			y2 = (m+1)*yy + y0;
			setColorPPS(send280pps_[1][dp]);
			glRectf(x1, y1, x2, y2);
			glRectf(x1, y1, x2, y2);	
			dp++;
		}
	}
	//	strain
	x1 = 0*xx + x0 + dx;
	y1 = 8*yy + y0 + dy;
	x2 = (2+1)*xx + x0;
	y2 = (8+0.5+1)*yy + y0;
	/*
	setColorStrain(tempStrain[1]);
	glRectf(x1, y1, x2, y2);
	*/
	setColorStrain(0.8*tempStrain[1]);
	glRectf(x1, y1, x2, y2);

	setColorStrain(tempStrain[1]);
	glRectf(x1+dx, y1+dy, x2-dy, y2-dy);
	/////////////////////////////////
	// LEGEND
	/////////////////////////////////
	//renderBitmapString(0.0f, 0.5f, 0.0f, INT_GLUT_BITMAP_HELVETICA_18 ,"kr neki");
		
	glEnd();
	glutSwapBuffers();

}

void initGlut(int argc, char **argv,
			  HandMeasPPS * DOUT, pthread_mutex_t * mutex)
{
	Meas_=DOUT;
	Mutex=mutex;
	glutInit(&argc, argv);
	glutInitWindowPosition(WINPOSX, WINPOSY);
	glutInitWindowSize(WINSIZEX, WINSIZEY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutWin=glutCreateWindow("Strain and PPS");
	//registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	//glutWMCloseFunc(exitGlut);
	glutKeyboardFunc(processNormalKeys);
	glutMainLoop();
}