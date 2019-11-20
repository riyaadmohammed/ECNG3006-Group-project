#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/L02_816008425.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/L02_816008425.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=C:/Studentlab2/L02_816008425/lcd.c C:/Studentlab2/L02_816008425/integration.c C:/Studentlab2/L02_816008425/scrolling.c "C:/Studentlab2/L02_816008425/shift reg.c"

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/808926576/lcd.o ${OBJECTDIR}/_ext/808926576/integration.o ${OBJECTDIR}/_ext/808926576/scrolling.o "${OBJECTDIR}/_ext/808926576/shift reg.o"
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/808926576/lcd.o.d ${OBJECTDIR}/_ext/808926576/integration.o.d ${OBJECTDIR}/_ext/808926576/scrolling.o.d "${OBJECTDIR}/_ext/808926576/shift reg.o.d"

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/808926576/lcd.o ${OBJECTDIR}/_ext/808926576/integration.o ${OBJECTDIR}/_ext/808926576/scrolling.o ${OBJECTDIR}/_ext/808926576/shift\ reg.o

# Source Files
SOURCEFILES=C:/Studentlab2/L02_816008425/lcd.c C:/Studentlab2/L02_816008425/integration.c C:/Studentlab2/L02_816008425/scrolling.c C:/Studentlab2/L02_816008425/shift reg.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/L02_816008425.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F452
MP_PROCESSOR_OPTION_LD=18f452
MP_LINKER_DEBUG_OPTION=-r=ROM@0x7DC0:0x7FFF -r=RAM@GPR:0x5F4:0x5FF -u_DEBUGSTACK
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/808926576/lcd.o: C:/Studentlab2/L02_816008425/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/808926576" 
	@${RM} ${OBJECTDIR}/_ext/808926576/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/808926576/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Studentlab2/L02_816008425" -I"C:/Program Files (x86)/Microchip/mplabc18/v3.47/h" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/include" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/portable/MPLAB/PIC18F" -ml -oa- -Ls -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/808926576/lcd.o   C:/Studentlab2/L02_816008425/lcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/808926576/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/808926576/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/808926576/integration.o: C:/Studentlab2/L02_816008425/integration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/808926576" 
	@${RM} ${OBJECTDIR}/_ext/808926576/integration.o.d 
	@${RM} ${OBJECTDIR}/_ext/808926576/integration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Studentlab2/L02_816008425" -I"C:/Program Files (x86)/Microchip/mplabc18/v3.47/h" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/include" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/portable/MPLAB/PIC18F" -ml -oa- -Ls -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/808926576/integration.o   C:/Studentlab2/L02_816008425/integration.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/808926576/integration.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/808926576/integration.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/808926576/scrolling.o: C:/Studentlab2/L02_816008425/scrolling.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/808926576" 
	@${RM} ${OBJECTDIR}/_ext/808926576/scrolling.o.d 
	@${RM} ${OBJECTDIR}/_ext/808926576/scrolling.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Studentlab2/L02_816008425" -I"C:/Program Files (x86)/Microchip/mplabc18/v3.47/h" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/include" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/portable/MPLAB/PIC18F" -ml -oa- -Ls -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/808926576/scrolling.o   C:/Studentlab2/L02_816008425/scrolling.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/808926576/scrolling.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/808926576/scrolling.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/808926576/shift\ reg.o: C:/Studentlab2/L02_816008425/shift\ reg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/808926576" 
	@${RM} "${OBJECTDIR}/_ext/808926576/shift reg.o".d 
	@${RM} "${OBJECTDIR}/_ext/808926576/shift reg.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Studentlab2/L02_816008425" -I"C:/Program Files (x86)/Microchip/mplabc18/v3.47/h" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/include" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/portable/MPLAB/PIC18F" -ml -oa- -Ls -o-  -I ${MP_CC_DIR}\\..\\h  -fo "${OBJECTDIR}/_ext/808926576/shift reg.o"   "C:/Studentlab2/L02_816008425/shift reg.c" 
	@${DEP_GEN} -d "${OBJECTDIR}/_ext/808926576/shift reg.o" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/808926576/shift reg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/_ext/808926576/lcd.o: C:/Studentlab2/L02_816008425/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/808926576" 
	@${RM} ${OBJECTDIR}/_ext/808926576/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/808926576/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Studentlab2/L02_816008425" -I"C:/Program Files (x86)/Microchip/mplabc18/v3.47/h" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/include" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/portable/MPLAB/PIC18F" -ml -oa- -Ls -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/808926576/lcd.o   C:/Studentlab2/L02_816008425/lcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/808926576/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/808926576/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/808926576/integration.o: C:/Studentlab2/L02_816008425/integration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/808926576" 
	@${RM} ${OBJECTDIR}/_ext/808926576/integration.o.d 
	@${RM} ${OBJECTDIR}/_ext/808926576/integration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Studentlab2/L02_816008425" -I"C:/Program Files (x86)/Microchip/mplabc18/v3.47/h" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/include" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/portable/MPLAB/PIC18F" -ml -oa- -Ls -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/808926576/integration.o   C:/Studentlab2/L02_816008425/integration.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/808926576/integration.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/808926576/integration.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/808926576/scrolling.o: C:/Studentlab2/L02_816008425/scrolling.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/808926576" 
	@${RM} ${OBJECTDIR}/_ext/808926576/scrolling.o.d 
	@${RM} ${OBJECTDIR}/_ext/808926576/scrolling.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Studentlab2/L02_816008425" -I"C:/Program Files (x86)/Microchip/mplabc18/v3.47/h" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/include" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/portable/MPLAB/PIC18F" -ml -oa- -Ls -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/808926576/scrolling.o   C:/Studentlab2/L02_816008425/scrolling.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/808926576/scrolling.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/808926576/scrolling.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/808926576/shift\ reg.o: C:/Studentlab2/L02_816008425/shift\ reg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/808926576" 
	@${RM} "${OBJECTDIR}/_ext/808926576/shift reg.o".d 
	@${RM} "${OBJECTDIR}/_ext/808926576/shift reg.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Studentlab2/L02_816008425" -I"C:/Program Files (x86)/Microchip/mplabc18/v3.47/h" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/include" -I"C:/Users/816008425/Downloads/FreeRTOSv10.2.1/FreeRTOS/Source/portable/MPLAB/PIC18F" -ml -oa- -Ls -o-  -I ${MP_CC_DIR}\\..\\h  -fo "${OBJECTDIR}/_ext/808926576/shift reg.o"   "C:/Studentlab2/L02_816008425/shift reg.c" 
	@${DEP_GEN} -d "${OBJECTDIR}/_ext/808926576/shift reg.o" 
	@${FIXDEPS} "${OBJECTDIR}/_ext/808926576/shift reg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/L02_816008425.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    C:/Studentlab2/L02_816008425/18f452.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "C:\Studentlab2\L02_816008425\18f452.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/L02_816008425.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/L02_816008425.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   C:/Studentlab2/L02_816008425/18f452.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "C:\Studentlab2\L02_816008425\18f452.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/L02_816008425.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
