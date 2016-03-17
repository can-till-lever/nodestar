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
CND_PLATFORM=GNU-Linux
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
	${OBJECTDIR}/AppAuth.o \
	${OBJECTDIR}/AppNodeStar.o \
	${OBJECTDIR}/Auth.o \
	${OBJECTDIR}/Cidr.o \
	${OBJECTDIR}/Server.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/model/DbRecUser.o \
	${OBJECTDIR}/model/UserAuth.o \
	${OBJECTDIR}/page/ShowAddresses.o \
	${OBJECTDIR}/page/SignIn.o \
	${OBJECTDIR}/page/Upload.o \
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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/AppAuth.o: AppAuth.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AppAuth.o AppAuth.cpp

${OBJECTDIR}/AppNodeStar.o: AppNodeStar.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AppNodeStar.o AppNodeStar.cpp

${OBJECTDIR}/Auth.o: Auth.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Auth.o Auth.cpp

${OBJECTDIR}/Cidr.o: Cidr.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Cidr.o Cidr.cpp

${OBJECTDIR}/Server.o: Server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Server.o Server.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/model/DbRecUser.o: model/DbRecUser.cpp 
	${MKDIR} -p ${OBJECTDIR}/model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/model/DbRecUser.o model/DbRecUser.cpp

${OBJECTDIR}/model/UserAuth.o: model/UserAuth.cpp 
	${MKDIR} -p ${OBJECTDIR}/model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/model/UserAuth.o model/UserAuth.cpp

${OBJECTDIR}/page/ShowAddresses.o: page/ShowAddresses.cpp 
	${MKDIR} -p ${OBJECTDIR}/page
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/page/ShowAddresses.o page/ShowAddresses.cpp

${OBJECTDIR}/page/SignIn.o: page/SignIn.cpp 
	${MKDIR} -p ${OBJECTDIR}/page
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/page/SignIn.o page/SignIn.cpp

${OBJECTDIR}/page/Upload.o: page/Upload.cpp 
	${MKDIR} -p ${OBJECTDIR}/page
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/page/Upload.o page/Upload.cpp

${OBJECTDIR}/utility/ImportNetworkCsv.o: utility/ImportNetworkCsv.cpp 
	${MKDIR} -p ${OBJECTDIR}/utility
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utility/ImportNetworkCsv.o utility/ImportNetworkCsv.cpp

${OBJECTDIR}/utility/ImportSmcXml.o: utility/ImportSmcXml.cpp 
	${MKDIR} -p ${OBJECTDIR}/utility
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utility/ImportSmcXml.o utility/ImportSmcXml.cpp

${OBJECTDIR}/utility/PopulateBasicIpAddresses.o: utility/PopulateBasicIpAddresses.cpp 
	${MKDIR} -p ${OBJECTDIR}/utility
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utility/PopulateBasicIpAddresses.o utility/PopulateBasicIpAddresses.cpp

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
