
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

#ifndef WINPTHREAD
#define WINPTHREAD

#include <setjmp.h>
#include <errno.h>
#include <sys/timeb.h>
#include <process.h>

typedef CRITICAL_SECTION pthread_mutex_t;
typedef HANDLE pthread_t;
typedef unsigned pthread_mutexattr_t;
typedef struct pthread_attr_t pthread_attr_t;

typedef struct _pthread_cleanup _pthread_cleanup;


static int pthread_mutex_init(pthread_mutex_t *m, pthread_mutexattr_t *a)
{
	(void) a;
	InitializeCriticalSection(m);

	return 0;
}

static HANDLE pthread_create(pthread_t *th, pthread_attr_t *attr, 
							_In_ void (__cdecl * func) (void *), void *arg)
{
	return (HANDLE)_beginthread(func, 0, NULL);
}
/*
static HANDLE pthread_create(pthread_t *th, pthread_attr_t *attr, 
							_In_ void *(__cdecl * func) (void *), void *arg)
{
	//return (HANDLE)_beginthread(func, 0, NULL);
}
*/
static int pthread_join(pthread_t pt, void * dva)
{
	return 0;
}

static int pthread_mutex_lock(pthread_mutex_t *m)
{
	EnterCriticalSection(m);
	return 0;
}

static int pthread_mutex_unlock(pthread_mutex_t *m)
{
	LeaveCriticalSection(m);
	return 0;
}

static int pthread_cancel(pthread_t t)
{
	return TerminateThread(t,0);
}
#endif