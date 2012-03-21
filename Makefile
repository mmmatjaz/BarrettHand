#######################################################################
#######################################################################
###	GENERAL
CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp app.cpp server.cpp bh280.cpp
#SOURCES=client_udp.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=RTserver
#EXECUTABLE=client
#######################################################################
#######################################################################
###	BHAND
BHAND_INSTALL_DIR = /usr/local/Barrett/BHand
#BHAND_INSTALL_DIR = /usr/local/Barrett/BHand2

#BHAND_DIR = ${BHAND_INSTALL_DIR}
INCLUDES += -I${BHAND_INSTALL_DIR}/thirdparty/include -I${BHAND_INSTALL_DIR}/API
LIBDIR += -L${BHAND_INSTALL_DIR}/API/lib
LDLIBS = -lBHand
CLEANCMD = rm -f ${PROGRAM} *.o
DEFS = -DLINUX 
############################
# Linux CAN Driver Dependency (must be included)
INCLUDES += -I../../thirdparty/peak-linux-driver-6.20/driver
INCLUDES += -I../../thirdparty/peak-linux-driver-6.20/lib
LDLIBS += -lpcan
############################
# Linux Serial Driver Dependency (optional)
#LDLIBS += -lctb-barrett-0.14
############################
# Last
LDLIBS += -lPocoFoundationBarrett
#######################################################################
#######################################################################
###	MAKE
default: ${OBJECTS}
	g++ ${OBJECTS} ${INCLUDES} ${LIBDIR} ${LDLIBS} -o ${EXECUTABLE}
${OBJECTS}: ${SOURCES}
	g++ ${DEFS} ${INCLUDES} -c ${SOURCES}

.PHONY: clean 

clean: 
	${CLEANCMD} 

