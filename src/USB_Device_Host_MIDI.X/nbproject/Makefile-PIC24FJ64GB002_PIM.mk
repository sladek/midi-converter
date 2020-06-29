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
ifeq "$(wildcard nbproject/Makefile-local-PIC24FJ64GB002_PIM.mk)" "nbproject/Makefile-local-PIC24FJ64GB002_PIM.mk"
include nbproject/Makefile-local-PIC24FJ64GB002_PIM.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC24FJ64GB002_PIM
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device_Host_MIDI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device_Host_MIDI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=usb/src/usb_device.c usb/src/usb_descriptors.c usb/src/usb_events.c usb/src/usb_host.c usb/src/usb_host_midi.c usb/src/usb_host_config.c src/app_host_midi_basic.c src/leds.c src/main.c src/system.c src/timer.c src/app_device_midi_basic.c midi_jack.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/usb/src/usb_device.o ${OBJECTDIR}/usb/src/usb_descriptors.o ${OBJECTDIR}/usb/src/usb_events.o ${OBJECTDIR}/usb/src/usb_host.o ${OBJECTDIR}/usb/src/usb_host_midi.o ${OBJECTDIR}/usb/src/usb_host_config.o ${OBJECTDIR}/src/app_host_midi_basic.o ${OBJECTDIR}/src/leds.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/timer.o ${OBJECTDIR}/src/app_device_midi_basic.o ${OBJECTDIR}/midi_jack.o
POSSIBLE_DEPFILES=${OBJECTDIR}/usb/src/usb_device.o.d ${OBJECTDIR}/usb/src/usb_descriptors.o.d ${OBJECTDIR}/usb/src/usb_events.o.d ${OBJECTDIR}/usb/src/usb_host.o.d ${OBJECTDIR}/usb/src/usb_host_midi.o.d ${OBJECTDIR}/usb/src/usb_host_config.o.d ${OBJECTDIR}/src/app_host_midi_basic.o.d ${OBJECTDIR}/src/leds.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/system.o.d ${OBJECTDIR}/src/timer.o.d ${OBJECTDIR}/src/app_device_midi_basic.o.d ${OBJECTDIR}/midi_jack.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/usb/src/usb_device.o ${OBJECTDIR}/usb/src/usb_descriptors.o ${OBJECTDIR}/usb/src/usb_events.o ${OBJECTDIR}/usb/src/usb_host.o ${OBJECTDIR}/usb/src/usb_host_midi.o ${OBJECTDIR}/usb/src/usb_host_config.o ${OBJECTDIR}/src/app_host_midi_basic.o ${OBJECTDIR}/src/leds.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/timer.o ${OBJECTDIR}/src/app_device_midi_basic.o ${OBJECTDIR}/midi_jack.o

# Source Files
SOURCEFILES=usb/src/usb_device.c usb/src/usb_descriptors.c usb/src/usb_events.c usb/src/usb_host.c usb/src/usb_host_midi.c usb/src/usb_host_config.c src/app_host_midi_basic.c src/leds.c src/main.c src/system.c src/timer.c src/app_device_midi_basic.c midi_jack.c


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
	${MAKE}  -f nbproject/Makefile-PIC24FJ64GB002_PIM.mk dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device_Host_MIDI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GB002
MP_LINKER_FILE_OPTION=,--script=p24FJ64GB002.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/usb/src/usb_device.o: usb/src/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_device.c  -o ${OBJECTDIR}/usb/src/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_device.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_descriptors.o: usb/src/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_descriptors.c  -o ${OBJECTDIR}/usb/src/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_descriptors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_descriptors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_events.o: usb/src/usb_events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_events.c  -o ${OBJECTDIR}/usb/src/usb_events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_events.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_events.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_host.o: usb/src/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_host.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_host.c  -o ${OBJECTDIR}/usb/src/usb_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_host.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_host.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_host_midi.o: usb/src/usb_host_midi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_host_midi.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_host_midi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_host_midi.c  -o ${OBJECTDIR}/usb/src/usb_host_midi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_host_midi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_host_midi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_host_config.o: usb/src/usb_host_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_host_config.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_host_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_host_config.c  -o ${OBJECTDIR}/usb/src/usb_host_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_host_config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_host_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_host_midi_basic.o: src/app_host_midi_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_host_midi_basic.o.d 
	@${RM} ${OBJECTDIR}/src/app_host_midi_basic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_host_midi_basic.c  -o ${OBJECTDIR}/src/app_host_midi_basic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_host_midi_basic.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_host_midi_basic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/leds.o: src/leds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/leds.o.d 
	@${RM} ${OBJECTDIR}/src/leds.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/leds.c  -o ${OBJECTDIR}/src/leds.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/leds.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/leds.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/system.o: src/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system.c  -o ${OBJECTDIR}/src/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/timer.o: src/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/timer.o.d 
	@${RM} ${OBJECTDIR}/src/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timer.c  -o ${OBJECTDIR}/src/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_device_midi_basic.o: src/app_device_midi_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_device_midi_basic.o.d 
	@${RM} ${OBJECTDIR}/src/app_device_midi_basic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_device_midi_basic.c  -o ${OBJECTDIR}/src/app_device_midi_basic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_device_midi_basic.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_device_midi_basic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/midi_jack.o: midi_jack.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/midi_jack.o.d 
	@${RM} ${OBJECTDIR}/midi_jack.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  midi_jack.c  -o ${OBJECTDIR}/midi_jack.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/midi_jack.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/midi_jack.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/usb/src/usb_device.o: usb/src/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_device.c  -o ${OBJECTDIR}/usb/src/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_device.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_descriptors.o: usb/src/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_descriptors.c  -o ${OBJECTDIR}/usb/src/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_descriptors.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_descriptors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_events.o: usb/src/usb_events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_events.c  -o ${OBJECTDIR}/usb/src/usb_events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_events.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_events.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_host.o: usb/src/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_host.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_host.c  -o ${OBJECTDIR}/usb/src/usb_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_host.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_host.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_host_midi.o: usb/src/usb_host_midi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_host_midi.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_host_midi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_host_midi.c  -o ${OBJECTDIR}/usb/src/usb_host_midi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_host_midi.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_host_midi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb/src/usb_host_config.o: usb/src/usb_host_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_host_config.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_host_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/src/usb_host_config.c  -o ${OBJECTDIR}/usb/src/usb_host_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb/src/usb_host_config.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_host_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_host_midi_basic.o: src/app_host_midi_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_host_midi_basic.o.d 
	@${RM} ${OBJECTDIR}/src/app_host_midi_basic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_host_midi_basic.c  -o ${OBJECTDIR}/src/app_host_midi_basic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_host_midi_basic.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_host_midi_basic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/leds.o: src/leds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/leds.o.d 
	@${RM} ${OBJECTDIR}/src/leds.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/leds.c  -o ${OBJECTDIR}/src/leds.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/leds.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/leds.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/system.o: src/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system.c  -o ${OBJECTDIR}/src/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/system.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/timer.o: src/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/timer.o.d 
	@${RM} ${OBJECTDIR}/src/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/timer.c  -o ${OBJECTDIR}/src/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/timer.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_device_midi_basic.o: src/app_device_midi_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_device_midi_basic.o.d 
	@${RM} ${OBJECTDIR}/src/app_device_midi_basic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_device_midi_basic.c  -o ${OBJECTDIR}/src/app_device_midi_basic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_device_midi_basic.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_device_midi_basic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/midi_jack.o: midi_jack.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/midi_jack.o.d 
	@${RM} ${OBJECTDIR}/midi_jack.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  midi_jack.c  -o ${OBJECTDIR}/midi_jack.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/midi_jack.o.d"        -g -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -fno-short-double -O1 -falign-arrays -I"." -DPIC24FJ64GB002_PIM -DNO_EZBL -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/midi_jack.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device_Host_MIDI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device_Host_MIDI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc"  -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=2048,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device_Host_MIDI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device_Host_MIDI.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_PIC24FJ64GB002_PIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"inc" -I"usb/inc" -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=2048,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device_Host_MIDI.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/PIC24FJ64GB002_PIM
	${RM} -r dist/PIC24FJ64GB002_PIM

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
