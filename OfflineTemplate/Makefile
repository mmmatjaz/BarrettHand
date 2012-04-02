
#######################################################################
#######################################################################
###	GENERAL
CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp app.cpp bh280.cpp graphics.cpp
#SOURCES=client_udp.c
OBJ=$(SOURCES:.cpp=.o)
ODIR=obj
OBJECTS = $(patsubst %,$(ODIR)/%,$(OBJ))

EXECUTABLE=../bin-linux/OfflineTemplate

#EXECUTABLE=client
#######################################################################
#######################################################################
###	BHAND
BHAND_INSTALL_DIR = /usr/local/Barrett/BHand
#BHAND_INSTALL_DIR = /usr/local/Barrett/BHand2

#BHAND_DIR = ${BHAND_INSTALL_DIR}
INCLUDES += -I${BHAND_INSTALL_DIR}/thirdparty/include -I${BHAND_INSTALL_DIR}/API
LIBDIR += -L${BHAND_INSTALL_DIR}/API/lib
LDLIBS = -lBHand -lglut -lGL -lGLU -lGLEW
CLEANCMD = rm -f ${PROGRAM} ./obj/*.o
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
	@echo Linkam
	g++ ${OBJECTS} ${INCLUDES} ${LIBDIR} ${LDLIBS} -o ${EXECUTABLE}
${OBJECTS}: ${SOURCES}
	@echo Kompajlam
	mkdir -p $(ODIR)
	g++ ${DEFS} ${INCLUDES} -c ${SOURCES} #-o ${OBJECTS}
	mv *.o ./obj/
.PHONY: clean 

clean: 
	${CLEANCMD} 

test:
	@echo "obj check"
