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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Cidr.o \
	${OBJECTDIR}/WebApp.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/utility/ImportNetworkCsv.o \
	${OBJECTDIR}/utility/ImportSmcXml.o \
	${OBJECTDIR}/utility/PopulateBasicIpAddresses.o


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
LDLIBSOPTIONS=-L/usr/local/lib -lcidr -lwt -lwtdbo -lwtdbopostgres -lwthttp

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Cidr.o: Cidr.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Cidr.o Cidr.cpp

${OBJECTDIR}/WebApp.o: WebApp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/WebApp.o WebApp.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/utility/ImportNetworkCsv.o: utility/ImportNetworkCsv.cpp 
	${MKDIR} -p ${OBJECTDIR}/utility
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utility/ImportNetworkCsv.o utility/ImportNetworkCsv.cpp

${OBJECTDIR}/utility/ImportSmcXml.o: utility/ImportSmcXml.cpp 
	${MKDIR} -p ${OBJECTDIR}/utility
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utility/ImportSmcXml.o utility/ImportSmcXml.cpp

${OBJECTDIR}/utility/PopulateBasicIpAddresses.o: utility/PopulateBasicIpAddresses.cpp 
	${MKDIR} -p ${OBJECTDIR}/utility
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utility/PopulateBasicIpAddresses.o utility/PopulateBasicIpAddresses.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
