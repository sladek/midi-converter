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
ifeq "$(wildcard nbproject/Makefile-local-PIC24FJ64GB002.mk)" "nbproject/Makefile-local-PIC24FJ64GB002.mk"
include nbproject/Makefile-local-PIC24FJ64GB002.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC24FJ64GB002
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../../../../../../framework/fileio/src/fileio.c ../../../../../../framework/usb/src/usb_host.c ../../../../../../framework/usb/src/usb_host_msd.c ../../../../../../framework/usb/src/usb_host_msd_scsi.c system.c main.c usb_config.c usb_host_midi.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1706142600/fileio.o ${OBJECTDIR}/_ext/838585624/usb_host.o ${OBJECTDIR}/_ext/838585624/usb_host_msd.o ${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o ${OBJECTDIR}/system.o ${OBJECTDIR}/main.o ${OBJECTDIR}/usb_config.o ${OBJECTDIR}/usb_host_midi.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1706142600/fileio.o.d ${OBJECTDIR}/_ext/838585624/usb_host.o.d ${OBJECTDIR}/_ext/838585624/usb_host_msd.o.d ${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o.d ${OBJECTDIR}/system.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/usb_config.o.d ${OBJECTDIR}/usb_host_midi.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1706142600/fileio.o ${OBJECTDIR}/_ext/838585624/usb_host.o ${OBJECTDIR}/_ext/838585624/usb_host_msd.o ${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o ${OBJECTDIR}/system.o ${OBJECTDIR}/main.o ${OBJECTDIR}/usb_config.o ${OBJECTDIR}/usb_host_midi.o

# Source Files
SOURCEFILES=../../../../../../framework/fileio/src/fileio.c ../../../../../../framework/usb/src/usb_host.c ../../../../../../framework/usb/src/usb_host_msd.c ../../../../../../framework/usb/src/usb_host_msd_scsi.c system.c main.c usb_config.c usb_host_midi.c


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
	${MAKE}  -f nbproject/Makefile-PIC24FJ64GB002.mk dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GB002
MP_LINKER_FILE_OPTION=,--script="../ezbl_integration/ezbl_build_standalone.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1706142600/fileio.o: ../../../../../../framework/fileio/src/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1706142600" 
	@${RM} ${OBJECTDIR}/_ext/1706142600/fileio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1706142600/fileio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../../framework/fileio/src/fileio.c  -o ${OBJECTDIR}/_ext/1706142600/fileio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1706142600/fileio.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1706142600/fileio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/838585624/usb_host.o: ../../../../../../framework/usb/src/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/838585624" 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../../framework/usb/src/usb_host.c  -o ${OBJECTDIR}/_ext/838585624/usb_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/838585624/usb_host.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/838585624/usb_host.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/838585624/usb_host_msd.o: ../../../../../../framework/usb/src/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/838585624" 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host_msd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../../framework/usb/src/usb_host_msd.c  -o ${OBJECTDIR}/_ext/838585624/usb_host_msd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/838585624/usb_host_msd.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/838585624/usb_host_msd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o: ../../../../../../framework/usb/src/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/838585624" 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../../framework/usb/src/usb_host_msd_scsi.c  -o ${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system.o: system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system.c  -o ${OBJECTDIR}/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb_config.o: usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_config.o.d 
	@${RM} ${OBJECTDIR}/usb_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_config.c  -o ${OBJECTDIR}/usb_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb_config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb_host_midi.o: usb_host_midi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_host_midi.o.d 
	@${RM} ${OBJECTDIR}/usb_host_midi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_host_midi.c  -o ${OBJECTDIR}/usb_host_midi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb_host_midi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb_host_midi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1706142600/fileio.o: ../../../../../../framework/fileio/src/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1706142600" 
	@${RM} ${OBJECTDIR}/_ext/1706142600/fileio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1706142600/fileio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../../framework/fileio/src/fileio.c  -o ${OBJECTDIR}/_ext/1706142600/fileio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1706142600/fileio.o.d"        -g -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1706142600/fileio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/838585624/usb_host.o: ../../../../../../framework/usb/src/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/838585624" 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../../framework/usb/src/usb_host.c  -o ${OBJECTDIR}/_ext/838585624/usb_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/838585624/usb_host.o.d"        -g -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/838585624/usb_host.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/838585624/usb_host_msd.o: ../../../../../../framework/usb/src/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/838585624" 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host_msd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../../framework/usb/src/usb_host_msd.c  -o ${OBJECTDIR}/_ext/838585624/usb_host_msd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/838585624/usb_host_msd.o.d"        -g -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/838585624/usb_host_msd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o: ../../../../../../framework/usb/src/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/838585624" 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../../framework/usb/src/usb_host_msd_scsi.c  -o ${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o.d"        -g -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/838585624/usb_host_msd_scsi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system.o: system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system.c  -o ${OBJECTDIR}/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system.o.d"        -g -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"        -g -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb_config.o: usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_config.o.d 
	@${RM} ${OBJECTDIR}/usb_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_config.c  -o ${OBJECTDIR}/usb_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb_config.o.d"        -g -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/usb_host_midi.o: usb_host_midi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_host_midi.o.d 
	@${RM} ${OBJECTDIR}/usb_host_midi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_host_midi.c  -o ${OBJECTDIR}/usb_host_midi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/usb_host_midi.o.d"        -g -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"." -I"../../../../../../framework" -I"../../../../../../framework/usb/inc" -I"../../../../../../framework/fileio/inc" -msmart-io=1 -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/usb_host_midi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../ezbl_integration/ezbl_lib.a  ../ezbl_integration/ezbl_build_standalone.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
###<EZBL MODIFIED 0>###
	@echo EZBL: Updating automatic sections in linker script
	$(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --gldbuilder  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo EZBL: Starting linking pass 1 of 2
	${MP_CC} $(MP_EXTRA_LD_PRE) -o "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_partial_link.elf"  ${OBJECTFILES_QUOTED_IF_SPACED}    ../ezbl_integration/ezbl_lib.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,-DEZBL_PASS_1_LINK,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=1000,--stack=16,--no-check-sections,--data-init,--pack-data,--no-handles,--no-isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-ivt,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  1>/dev/null
	$(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=1 -elf_artifact="/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_partial_link.elf"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo EZBL: Starting linking pass 2 of 2
###</EZBL MODIFIED 0>###
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX)  ${OBJECTFILES_QUOTED_IF_SPACED}    ../ezbl_integration/ezbl_lib.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=1000,--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-ivt,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
###<EZBL MODIFIED 0>###
#	Run the ezbl_tools.jar dump processor again so we can test if the Bootloader geometry changed
	@echo $(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=2 -elf_artifact="dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX)"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo 	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX)  ${OBJECTFILES_QUOTED_IF_SPACED}    ../ezbl_integration/ezbl_lib.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=1000,--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-ivt,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  | $(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=2 -elf_artifact="dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX)"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	$(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --blobber -generate_merge  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@test -e "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_partial_link.elf" && rm "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_partial_link.elf" || true
###</EZBL MODIFIED 0>###
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../ezbl_integration/ezbl_lib.a ../ezbl_integration/ezbl_build_standalone.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
###<EZBL MODIFIED 0>###
	@echo EZBL: Updating automatic sections in linker script
	$(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --gldbuilder  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo EZBL: Starting linking pass 1 of 2
	${MP_CC} $(MP_EXTRA_LD_PRE) -o "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_partial_link.elf"  ${OBJECTFILES_QUOTED_IF_SPACED}    ../ezbl_integration/ezbl_lib.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,-DEZBL_PASS_1_LINK,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=1000,--stack=16,--no-check-sections,--data-init,--pack-data,--no-handles,--no-isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-ivt,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  1>/dev/null
	$(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=1 -elf_artifact="/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_partial_link.elf"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo EZBL: Starting linking pass 2 of 2
###</EZBL MODIFIED 0>###
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../ezbl_integration/ezbl_lib.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=1000,--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-ivt,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
###<EZBL MODIFIED 0>###
#	Run the ezbl_tools.jar dump processor again so we can test if the Bootloader geometry changed
	@echo $(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=2 -elf_artifact="dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo 	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../ezbl_integration/ezbl_lib.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DEZBL_BOOT_PROJECT=usb_msd -DXPRJ_PIC24FJ64GB002=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=1000,--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-ivt,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  | $(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=2 -elf_artifact="dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	$(MP_JAVA_PATH)java -cp "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --blobber -generate_merge  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@test -e "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_partial_link.elf" && rm "/home/sladekm/Projects/Microcontrolers/USBH_DIN-MIDI_Converter/src/ezbl-v2.11/ex_boot_usb_msd_v2016_11_07/apps/usb/host/ex_boot_usb_msd/firmware/ezbl_integration/ezbl_partial_link.elf" || true
###</EZBL MODIFIED 0>###
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/exp16_pic24fj64gb002_pim.x.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/PIC24FJ64GB002
	${RM} -r dist/PIC24FJ64GB002

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
