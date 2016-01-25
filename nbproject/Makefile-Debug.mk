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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/e82ab63d/AppNodeStar.o \
	${OBJECTDIR}/_ext/e82ab63d/Server.o \
	${OBJECTDIR}/_ext/400e14d7/DbRecUser.o \
	${OBJECTDIR}/_ext/400e14d7/UserAuth.o \
	${OBJECTDIR}/Cidr.o \
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
LDLIBSOPTIONS=-L/usr/local/lib -lcidr -lwt -lwtdbo -lwtdbopostgres -lwthttp -lboost_system-gcc48-mt-1_58

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodestar ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/e82ab63d/AppNodeStar.o: /home/sysadmin/projects/nodestar/AppNodeStar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e82ab63d
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e82ab63d/AppNodeStar.o /home/sysadmin/projects/nodestar/AppNodeStar.cpp

${OBJECTDIR}/_ext/e82ab63d/Server.o: /home/sysadmin/projects/nodestar/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e82ab63d
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e82ab63d/Server.o /home/sysadmin/projects/nodestar/Server.cpp

${OBJECTDIR}/_ext/400e14d7/DbRecUser.o: /home/sysadmin/projects/nodestar/model/DbRecUser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/400e14d7
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/400e14d7/DbRecUser.o /home/sysadmin/projects/nodestar/model/DbRecUser.cpp

${OBJECTDIR}/_ext/400e14d7/UserAuth.o: /home/sysadmin/projects/nodestar/model/UserAuth.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/400e14d7
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/400e14d7/UserAuth.o /home/sysadmin/projects/nodestar/model/UserAuth.cpp

${OBJECTDIR}/Cidr.o: Cidr.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Cidr.o Cidr.cpp

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
