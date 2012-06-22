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