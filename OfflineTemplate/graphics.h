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

#include "app.h"

#ifdef __linux__
#include <GL/freeglut.h>
//#include <GL/gl.h>
#endif

#ifdef _WIN32 || defined _WIN64
#include <GL\glut.h>
#endif

void initGlut(int argc, char **argv,
			  HandMeasPPS * DOUT, pthread_mutex_t * mutex);

/*
class Graphics
{
private:
	HandMeasPPS pMeas;
	HandMeasPPS * Meas_;
	pthread_mutex_t * Mutex;
	bool shouldRun;
	void processNormalKeys(unsigned char key, int x, int y);
	void changeSize(int w, int h);
	void setColorPPS(int index);
	void setColorStrain(float index);
	void renderBitmapString(float x,float y,float z,void *font,char *string);
	void renderScene(void);

public:
	void initGlut(int argc, char **argv);
};
*/