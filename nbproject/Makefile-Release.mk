#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/BHTree.o \
	${OBJECTDIR}/IIntegrator.o \
	${OBJECTDIR}/IModel.o \
	${OBJECTDIR}/IntegratorADB2.o \
	${OBJECTDIR}/IntegratorADB3.o \
	${OBJECTDIR}/IntegratorADB4.o \
	${OBJECTDIR}/IntegratorADB5.o \
	${OBJECTDIR}/IntegratorADB6.o \
	${OBJECTDIR}/IntegratorEuler.o \
	${OBJECTDIR}/IntegratorEulerImproved.o \
	${OBJECTDIR}/IntegratorHeun.o \
	${OBJECTDIR}/IntegratorRK3.o \
	${OBJECTDIR}/IntegratorRK4.o \
	${OBJECTDIR}/IntegratorRK5.o \
	${OBJECTDIR}/IntegratorRKF4.o \
	${OBJECTDIR}/ModelNBody.o \
	${OBJECTDIR}/NBodyWnd.o \
	${OBJECTDIR}/SDLWnd.o \
	${OBJECTDIR}/Types.o \
	${OBJECTDIR}/Vector.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wall -fopenmp -O3 -std=c++11
CXXFLAGS=-Wall -fopenmp -O3 -std=c++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu -lSDL -lSDL_ttf -lGL -lGLU -lX11

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/barnes-hut-simulator

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/barnes-hut-simulator: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/barnes-hut-simulator ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/BHTree.o: BHTree.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BHTree.o BHTree.cpp

${OBJECTDIR}/IIntegrator.o: IIntegrator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IIntegrator.o IIntegrator.cpp

${OBJECTDIR}/IModel.o: IModel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IModel.o IModel.cpp

${OBJECTDIR}/IntegratorADB2.o: IntegratorADB2.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorADB2.o IntegratorADB2.cpp

${OBJECTDIR}/IntegratorADB3.o: IntegratorADB3.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorADB3.o IntegratorADB3.cpp

${OBJECTDIR}/IntegratorADB4.o: IntegratorADB4.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorADB4.o IntegratorADB4.cpp

${OBJECTDIR}/IntegratorADB5.o: IntegratorADB5.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorADB5.o IntegratorADB5.cpp

${OBJECTDIR}/IntegratorADB6.o: IntegratorADB6.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorADB6.o IntegratorADB6.cpp

${OBJECTDIR}/IntegratorEuler.o: IntegratorEuler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorEuler.o IntegratorEuler.cpp

${OBJECTDIR}/IntegratorEulerImproved.o: IntegratorEulerImproved.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorEulerImproved.o IntegratorEulerImproved.cpp

${OBJECTDIR}/IntegratorHeun.o: IntegratorHeun.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorHeun.o IntegratorHeun.cpp

${OBJECTDIR}/IntegratorRK3.o: IntegratorRK3.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorRK3.o IntegratorRK3.cpp

${OBJECTDIR}/IntegratorRK4.o: IntegratorRK4.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorRK4.o IntegratorRK4.cpp

${OBJECTDIR}/IntegratorRK5.o: IntegratorRK5.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorRK5.o IntegratorRK5.cpp

${OBJECTDIR}/IntegratorRKF4.o: IntegratorRKF4.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IntegratorRKF4.o IntegratorRKF4.cpp

${OBJECTDIR}/ModelNBody.o: ModelNBody.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ModelNBody.o ModelNBody.cpp

${OBJECTDIR}/NBodyWnd.o: NBodyWnd.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NBodyWnd.o NBodyWnd.cpp

${OBJECTDIR}/SDLWnd.o: SDLWnd.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SDLWnd.o SDLWnd.cpp

${OBJECTDIR}/Types.o: Types.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Types.o Types.cpp

${OBJECTDIR}/Vector.o: Vector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Vector.o Vector.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -s -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/barnes-hut-simulator

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
