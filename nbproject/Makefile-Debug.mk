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
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/IntegratorEuler.o \
	${OBJECTDIR}/IntegratorRK3.o \
	${OBJECTDIR}/IntegratorADB6.o \
	${OBJECTDIR}/BHTree.o \
	${OBJECTDIR}/IntegratorHeun.o \
	${OBJECTDIR}/SDLWnd.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/NBodyWnd.o \
	${OBJECTDIR}/Vector.o \
	${OBJECTDIR}/IIntegrator.o \
	${OBJECTDIR}/IntegratorADB4.o \
	${OBJECTDIR}/IModel.o \
	${OBJECTDIR}/IntegratorEulerImproved.o \
	${OBJECTDIR}/IntegratorADB3.o \
	${OBJECTDIR}/Types.o \
	${OBJECTDIR}/IntegratorADB2.o \
	${OBJECTDIR}/IntegratorRK5.o \
	${OBJECTDIR}/IntegratorADB5.o \
	${OBJECTDIR}/ModelNBody.o \
	${OBJECTDIR}/IntegratorRK4.o \
	${OBJECTDIR}/IntegratorRKF4.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/barnes_hut

dist/Debug/GNU-Linux-x86/barnes_hut: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -lSDL -lSDL_ttf -lGL -lGLU -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/barnes_hut ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/IntegratorEuler.o: nbproject/Makefile-${CND_CONF}.mk IntegratorEuler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorEuler.o IntegratorEuler.cpp

${OBJECTDIR}/IntegratorRK3.o: nbproject/Makefile-${CND_CONF}.mk IntegratorRK3.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorRK3.o IntegratorRK3.cpp

${OBJECTDIR}/IntegratorADB6.o: nbproject/Makefile-${CND_CONF}.mk IntegratorADB6.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorADB6.o IntegratorADB6.cpp

${OBJECTDIR}/BHTree.o: nbproject/Makefile-${CND_CONF}.mk BHTree.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/BHTree.o BHTree.cpp

${OBJECTDIR}/IntegratorHeun.o: nbproject/Makefile-${CND_CONF}.mk IntegratorHeun.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorHeun.o IntegratorHeun.cpp

${OBJECTDIR}/SDLWnd.o: nbproject/Makefile-${CND_CONF}.mk SDLWnd.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/SDLWnd.o SDLWnd.cpp

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/NBodyWnd.o: nbproject/Makefile-${CND_CONF}.mk NBodyWnd.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/NBodyWnd.o NBodyWnd.cpp

${OBJECTDIR}/Vector.o: nbproject/Makefile-${CND_CONF}.mk Vector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/Vector.o Vector.cpp

${OBJECTDIR}/IIntegrator.o: nbproject/Makefile-${CND_CONF}.mk IIntegrator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IIntegrator.o IIntegrator.cpp

${OBJECTDIR}/IntegratorADB4.o: nbproject/Makefile-${CND_CONF}.mk IntegratorADB4.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorADB4.o IntegratorADB4.cpp

${OBJECTDIR}/IModel.o: nbproject/Makefile-${CND_CONF}.mk IModel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IModel.o IModel.cpp

${OBJECTDIR}/IntegratorEulerImproved.o: nbproject/Makefile-${CND_CONF}.mk IntegratorEulerImproved.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorEulerImproved.o IntegratorEulerImproved.cpp

${OBJECTDIR}/IntegratorADB3.o: nbproject/Makefile-${CND_CONF}.mk IntegratorADB3.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorADB3.o IntegratorADB3.cpp

${OBJECTDIR}/Types.o: nbproject/Makefile-${CND_CONF}.mk Types.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/Types.o Types.cpp

${OBJECTDIR}/IntegratorADB2.o: nbproject/Makefile-${CND_CONF}.mk IntegratorADB2.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorADB2.o IntegratorADB2.cpp

${OBJECTDIR}/IntegratorRK5.o: nbproject/Makefile-${CND_CONF}.mk IntegratorRK5.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorRK5.o IntegratorRK5.cpp

${OBJECTDIR}/IntegratorADB5.o: nbproject/Makefile-${CND_CONF}.mk IntegratorADB5.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorADB5.o IntegratorADB5.cpp

${OBJECTDIR}/ModelNBody.o: nbproject/Makefile-${CND_CONF}.mk ModelNBody.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/ModelNBody.o ModelNBody.cpp

${OBJECTDIR}/IntegratorRK4.o: nbproject/Makefile-${CND_CONF}.mk IntegratorRK4.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorRK4.o IntegratorRK4.cpp

${OBJECTDIR}/IntegratorRKF4.o: nbproject/Makefile-${CND_CONF}.mk IntegratorRKF4.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/IntegratorRKF4.o IntegratorRKF4.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/barnes_hut

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
