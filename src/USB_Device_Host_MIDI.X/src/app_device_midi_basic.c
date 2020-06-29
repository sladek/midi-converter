/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
 *******************************************************************************/

/** INCLUDES *******************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "system.h"

#include "usb.h"
#include "usb_device_midi.h"

#include"leds.h"


/** VARIABLES ******************************************************/
/* Some processors have a limited range of RAM addresses where the USB module
 * is able to access.  The following section is for those devices.  This section
 * assigns the buffers that need to be used by the USB module into those
 * specific areas.
 */
#if defined(FIXED_ADDRESS_MEMORY)
#if defined(COMPILER_MPLAB_C18)
#pragma udata DEVICE_AUDIO_MIDI_RX_DATA_BUFFER=DEVCE_AUDIO_MIDI_RX_DATA_BUFFER_ADDRESS
static uint8_t ReceivedDataBuffer[64];
#pragma udata DEVICE_AUDIO_MIDI_EVENT_DATA_BUFFER=DEVCE_AUDIO_MIDI_EVENT_DATA_BUFFER_ADDRESS
static USB_AUDIO_MIDI_EVENT_PACKET midiData;
#pragma udata
#elif defined(__XC8)
static uint8_t ReceivedDataBuffer[64] DEVCE_AUDIO_MIDI_RX_DATA_BUFFER_ADDRESS;
static USB_AUDIO_MIDI_EVENT_PACKET midiData DEVCE_AUDIO_MIDI_EVENT_DATA_BUFFER_ADDRESS;
#endif
#else
#define MIDI_USB_BUFFER_SIZE                (uint8_t)4
#define MIDI_UART_BUFFER_SIZE (uint8_t)64

static uint8_t ReceivedDataBuffer[MIDI_UART_BUFFER_SIZE];
static USB_AUDIO_MIDI_EVENT_PACKET midiData;
#endif

// MIDI packet used to translate MIDI UART to MIDI USB, with flag
USB_AUDIO_MIDI_EVENT_PACKET UARTDeviceTranslatedToUSB;
extern bool        UARTmidiPacketTranslated;
// MIDI packet used to translate MIDI UART to MIDI USB, with flag
extern uint32_t UARTTranslatedToUSB;

//A circular buffer for storing received bytes from the MIDI (UART), to be sent do attached USB MIDI device
uint8_t        MIDIDeviceInRxBuffer[MIDI_UART_BUFFER_SIZE];
uint8_t*       pRxUARTBufDeviceReadLocation;
uint8_t*       pRxUARTBufDeviceWriteLocation;
uint8_t        UARTDeviceInByteCount;

// uint8_t        MIDIDeviceRxCommandState;
bool           DeviceSysExCommandPending;
bool           DeviceBytePending;
//uint8_t        DeviceRxRunningStatusByte;
//uint8_t        DeviceTxRunningStatusByte;

// Read location of the buffer from which the data is read;
static uint8_t*       pTxBufReadLocation;


static USB_HANDLE USBTxHandle;
static USB_HANDLE USBRxHandle;

static uint8_t pitch;
static bool sentNoteOff;

static USB_VOLATILE uint8_t msCounter;

void MIDI_Device_OutJackTasks(void);
void MIDI_InJackTasks(void);
void sendMessage(uint8_t*, uint8_t);

/*********************************************************************
 * Function: void APP_DeviceAudioMIDIInitialize(void);
 *
 * Overview: Initializes the demo code
 *
 * PreCondition: None
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
void APP_DeviceAudioMIDIInitialize() {
    USBTxHandle = NULL;
    USBRxHandle = NULL;


    // Setup circular FIFO for MIDI In port
    UARTDeviceInByteCount = 0;
    pRxUARTBufDeviceReadLocation = &MIDIDeviceInRxBuffer[0];
    pRxUARTBufDeviceWriteLocation = &MIDIDeviceInRxBuffer[0];
    
    // Clear UART translation packet and real-time packet/buffer
    UARTDeviceTranslatedToUSB.Val = 0;
    
    // Clear status flags
    DeviceSysExCommandPending = false;
    UARTmidiPacketTranslated = false;

    //enable the HID endpoint
    USBEnableEndpoint(USB_DEVICE_AUDIO_MIDI_ENDPOINT, USB_OUT_ENABLED | USB_IN_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);

    //Re-arm the OUT endpoint for the next packet
    USBRxHandle = USBRxOnePacket(USB_DEVICE_AUDIO_MIDI_ENDPOINT, (uint8_t*) & ReceivedDataBuffer, 64);


    MIDI_Jack_Initialize();



}

/*********************************************************************
 * Function: void APP_DeviceAudioMIDIInitialize(void);
 *
 * Overview: Initializes the demo code
 *
 * PreCondition: None
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
void APP_DeviceAudioMIDISOFHandler() {
    if (msCounter != 0) {
        msCounter--;
    }
}

/*********************************************************************
 * Function: void APP_DeviceAudioMIDITasks(void);
 *
 * Overview: Keeps the Custom HID demo running.
 *
 * PreCondition: The demo should have been initialized and started via
 *   the APP_DeviceAudioMIDIInitialize() and APP_DeviceAudioMIDIStart() demos
 *   respectively.
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
void APP_DeviceAudioMIDITasks() {
    /* If the device is not configured yet, or the device is suspended, then
     * we don't need to run the demo since we can't send any data.
     */
    if ((USBGetDeviceState() < CONFIGURED_STATE) ||
            (USBIsDeviceSuspended() == true)) {
        return;
    }

    MIDI_InJackTasks();
    // If UART packet is completely translated it can be sent to USB MIDI 
    if(UARTmidiPacketTranslated == true){
        USBTxHandle = USBTxOnePacket(USB_DEVICE_AUDIO_MIDI_ENDPOINT,(uint8_t*)&UARTTranslatedToUSB,4);
        UARTmidiPacketTranslated = false;
    }


    if (!USBHandleBusy(USBRxHandle)) {
        //We have received a MIDI packet from the host, process it and then
        //  prepare to receive the next packet

        //INSERT MIDI PROCESSING CODE HERE
        MIDI_Device_OutJackTasks();
        //Get ready for next packet (this will overwrite the old data)
        USBRxHandle = USBRxOnePacket(USB_DEVICE_AUDIO_MIDI_ENDPOINT, (uint8_t*) & ReceivedDataBuffer, 64);
    }
}

void MIDI_Device_OutJackTasks(void) {

    pTxBufReadLocation = &ReceivedDataBuffer[0];
    
    LED1_Set_State(LED_STATUS_BLINK_ONCE);
    while (pTxBufReadLocation < &ReceivedDataBuffer[MIDI_UART_BUFFER_SIZE]) {
        // There is something in the buffer
        uint8_t cin = *pTxBufReadLocation & 0x0F;
        switch (cin) {
            case MIDI_CIN_2_uint8_t_MESSAGE:
            case MIDI_CIN_SYSEX_ENDS_2:
            case MIDI_CIN_PROGRAM_CHANGE:
            case MIDI_CIN_CHANNEL_PREASURE:
                sendMessage(pTxBufReadLocation, 2);
//                pTxBufReadLocation = pTxBufReadLocation + 4;
                break;
            case MIDI_CIN_SYSEX_ENDS_1:
            case MIDI_CIN_SINGLE_uint8_t:
                sendMessage(pTxBufReadLocation, 1);
//                pTxBufReadLocation = pTxBufReadLocation + 4;
                break;
            case MIDI_CIN_3_uint8_t_MESSAGE:
            case MIDI_CIN_SYSEX_START:
            case MIDI_CIN_SYSEX_ENDS_3:
            case MIDI_CIN_NOTE_OFF:
            case MIDI_CIN_NOTE_ON:
            case MIDI_CIN_POLY_KEY_PRESS:
            case MIDI_CIN_CONTROL_CHANGE:
            case MIDI_CIN_PITCH_BEND_CHANGE:
                sendMessage(pTxBufReadLocation, 3);
//                pTxBufReadLocation = pTxBufReadLocation + 4;
                break;
            default:
                pTxBufReadLocation = pTxBufReadLocation + 4;
                break;
        }
    }
    // let's clear the buffer
    pTxBufReadLocation = &ReceivedDataBuffer[0];
    int i;
    for (i = 0; i < MIDI_UART_BUFFER_SIZE; i++) {
        *pTxBufReadLocation++ = 0x00;
    }
}

/**
 * Sends characters from MIDI buffer to serial port. It skips the first character as it is CIN 
 * @param buffer MIDI buffer
 * @param length Number of characters to be sent
 */
void sendMessage(uint8_t* buffer, uint8_t length) {
    uint8_t i;
    //    if( (*(buffer+1) == 0xB0) && (*(buffer+2) == 0x06) ){
    //        *(buffer+3) = 0x0F;
    //    }
    uint8_t* readFrom = buffer+1;
    for (i = 0; i < length; i++) {
        while (U2STAbits.TRMT == 0); // Wait for UART to be ready
        U2TXREG = *readFrom++;
    }
    pTxBufReadLocation +=4;
}

//-----------------------------------------------------------------------------
//Check the MIDI UART FIFO buffer to see if any MIDI traffic was received by
// the interrupt. If so, we need to copy it from the UART FIFO buffer to the USB FIFO buffer,
// and translate it into the USB MIDI packet.
//-----------------------------------------------------------------------------
/*
void MIDI_Device_InJackTasks(void)
{
    static int8_t ExpectedDataBytes;
    static int8_t WriteIndex;
    
    //If we have something within the UART MIDI Rx Buffer
    if(UARTDeviceInByteCount > 0)
    {
        //Check if we received a new status (MIDI command) byte.  If so, regardless of previous state
        //jump to the state where we are starting to receive a new MIDI command packet.
        if(*pRxUARTBufDeviceReadLocation & 0x80) 
        {
            MIDIDeviceRxCommandState = STATE_WAIT_STATUS_BYTE;
            DeviceRxRunningStatusByte = *pRxUARTBufDeviceReadLocation;   //Save the Status byte.
            
            //Reset write index pointer to to start of MIDI_x field of USB packet buffer,
            //unless we just recevied the MIDI_STATUS_SYSEX_END, where we will still need the old
            //WriteIndex value to know the proper CIN byte value of the next USB packet.
            if(*pRxUARTBufDeviceReadLocation != MIDI_STATUS_SYSEX_END)
            {
                WriteIndex = 2;
                UARTDeviceTranslatedToUSB.Val = 0;   //All pad bits/bytes = 0 according to MIDI-Audio USB specs.
            }// if(*pRxUARTBufDeviceReadLocation != MIDI_STATUS_SYSEX_END)
            
            //We are eventually going to consume the byte we just received, 
            //before returning in this scenario.
            pRxUARTBufDeviceReadLocation++;
            UARTDeviceInByteCount--;
            if((unsigned int)pRxUARTBufDeviceReadLocation >= ((unsigned int)&MIDIDeviceInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
            {
                pRxUARTBufDeviceReadLocation = &MIDIDeviceInRxBuffer[0];
            }
        }// if(*pRxUARTBufDeviceReadLocation & 0x80) 
        else if(MIDIDeviceRxCommandState == STATE_WAIT_STATUS_BYTE)   //Now check for "Running Status"
        {
            //If we get to here, this means we just received the first data byte,
            //that was part of a running status command.
            //In this case, we don't want to consume the ReceivedByte yet.  For this
            //iteration of this [if(UARTDeviceInByteCount > 0)] code, we just need to re-process 
            //the previous DeviceRxRunningStatusByte (ex: don't write to buffer here).  Then on the
            //next iteration we will actually consume the newest ReceivedByte.
        }// else if(MIDIDeviceRxCommandState == STATE_WAIT_STATUS_BYTE)
        else
        {
            //We are going to consume the (non-status) byte we just received, in this scenario.
        }// else
        
        //Check if we already received the Status (MIDI command) byte already, and
        //are currently waiting/expecting to receive data bytes.
        if(MIDIDeviceRxCommandState == STATE_WAITING_BYTES)
        {
            //Check if we are handling a SysEx command, or a normal command.
            if(DeviceSysExCommandPending == true)
            {
                //Handle the variable data length SysEx command.
                *((int8_t*)&UARTDeviceTranslatedToUSB + WriteIndex) = *pRxUARTBufDeviceReadLocation;
                pRxUARTBufDeviceReadLocation++;
                UARTDeviceInByteCount--;
                if((unsigned int)pRxUARTBufDeviceReadLocation >= ((unsigned int)&MIDIDeviceInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                {
                    pRxUARTBufDeviceReadLocation = &MIDIDeviceInRxBuffer[0];
                }
                if(WriteIndex < 3u)
                {
                    WriteIndex++;
                }// if(WriteIndex < 3u)
                else if(WriteIndex == 3)
                {
                    //We have accumulated a full USB-MIDI packet worth of data.
                    //Prepare state machine to send the packet.
                    UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_START; //Might also be continuing instead of starting.
                    
                    UARTMidiDevicePacketTranslated = true; //Set boolean flag here to let USB code know it should prepare a packet.
                    WriteIndex = 1; //Move write pointer/index back to start of MIDI_x field of USB packet buffer
                }// else if(WriteIndex == 3)                 
            }// if(DeviceSysExCommandPending == TRUE)
            else
            {    
                *(int8_t*)(((int8_t*)&UARTDeviceTranslatedToUSB) + WriteIndex) = *pRxUARTBufDeviceReadLocation;
                pRxUARTBufDeviceReadLocation++;
                UARTDeviceInByteCount--;
                if((unsigned int)pRxUARTBufDeviceReadLocation >= ((unsigned int)&MIDIDeviceInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                {
                    pRxUARTBufDeviceReadLocation = &MIDIDeviceInRxBuffer[0];
                }
                if(ExpectedDataBytes != 0)
                {
                    WriteIndex++;
                    ExpectedDataBytes--;
                    //Check if we finished receiving all expected bytes in the MIDI command
                    if(ExpectedDataBytes == 0)
                    {
                        UARTMidiDevicePacketTranslated = true; //Set boolean flag here so we know to add packet to buffer OUT
                        MIDIDeviceRxCommandState = STATE_WAIT_STATUS_BYTE;                
                    }// if(ExpectedDataBytes == 0)
                }// if(ExpectedDataBytes != 0)
            }// else
        }// if(MIDIDeviceRxCommandState == STATE_WAITING_BYTES)
        else if(MIDIDeviceRxCommandState == STATE_WAIT_STATUS_BYTE)
        {
            ExpectedDataBytes = 0;  //Will get updated with correct value, once we have parsed the status byte and know how many data bytes to expect.
            MIDIDeviceRxCommandState = STATE_WAITING_BYTES;   //Initialize UART recevier state machine
            
            //We presumably have just received a status byte (MIDI command byte)
            if(DeviceRxRunningStatusByte & 0x80) //Check if we received a new status (MIDI command) byte, indicated by bit 7 of UART byte being set
            {
                //Check if it was a system common command of some kind (upper nibble = 0xF)
                if((DeviceRxRunningStatusByte & 0xF0) == 0xF0) 
                {
                    UARTDeviceTranslatedToUSB.MIDI_0 = DeviceRxRunningStatusByte;
                    //Check for system common commands
                    switch(DeviceRxRunningStatusByte)
                    {
                        case(MIDI_STATUS_SYSEX_START):
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_START;
                            ExpectedDataBytes = 255;    //Not really expecting 255.  SysEx is variable/unknown length.
                            DeviceSysExCommandPending = true; //Let state machine know to perform special handling for this command.
                            break;
                        case(MIDI_STATUS_SYSEX_END):
                            //This case is special, since the actual data payload size is not fixed.
                            //However, when we get this byte, this means the SysEx command is completing,
                            //and therefore we can determine the residual number of bytes we need to send
                            //based on the WriteIndex variable.
                            if(WriteIndex == 1)
                                UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_ENDS_1;
                            else if(WriteIndex == 2)
                                UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_ENDS_2;
                            else if(WriteIndex == 3)
                                UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_ENDS_3;
                            //Complete the request here, rather than the normal handler.
                            WriteIndex = 2; //Move write pointer/index back to start of buffer for next MIDI command
                            UARTMidiDevicePacketTranslated = true; //Set boolean flag here to let USB code know it should prepare a packet.
                            MIDIDeviceRxCommandState = STATE_WAIT_STATUS_BYTE;  
                            DeviceSysExCommandPending = false;        //No longer handling a SysEx data flow.
                            ExpectedDataBytes = 0;
                            break;
                        case(MIDI_STATUS_MTC_QFRAME):
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_2_BYTE_MESSAGE;
                            ExpectedDataBytes = 1;
                            break;
                        case(MIDI_STATUS_SONG_POSITION):
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_3_BYTE_MESSAGE;
                            ExpectedDataBytes = 2;
                            break;
                        case(MIDI_STATUS_SONG_SELECT):
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_2_BYTE_MESSAGE;
                            ExpectedDataBytes = 1;
                            break;
                        case(MIDI_STATUS_TUNE_REQUEST):
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_1_BYTE_MESSAGE;
                            ExpectedDataBytes = 0;
                            break;
                        default:
                            MIDIDeviceRxCommandState = STATE_WAIT_STATUS_BYTE;
                            ExpectedDataBytes = 0;                            
                            break;
                    }//switch(DeviceRxRunningStatusByte)    
                    
                    //Check if we finished receiving all expected bytes in the MIDI command.
                    //This would be the case for system commands that consist only of the status byte. 
                    if((ExpectedDataBytes == 0) && (MIDIDeviceRxCommandState = STATE_WAITING_BYTES))  //Not note really waiting for extra bytes in this scenario.
                    {
                        //Prepare state machine to prepare USB transmit packet.
                        UARTMidiDevicePacketTranslated = true; //Set boolean flag here to let USB code know it should prepare a packet.
                        MIDIDeviceRxCommandState = STATE_WAIT_STATUS_BYTE;                
                    } 
                }//if((DeviceRxRunningStatusByte & 0xF0) == 0xF0) 
                else
                {
                    //Must have been a normal MIDI command status byte
                    UARTDeviceTranslatedToUSB.MIDI_0 = DeviceRxRunningStatusByte;
                    WriteIndex = 2;
                    DeviceRxRunningStatusByte &= 0xF0;   //Strip off the channel bits to get the command by itself
                    switch(DeviceRxRunningStatusByte)
                    {
                        case(MIDI_CIN_NOTE_OFF << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_NOTE_OFF;
                            break;
                        case(MIDI_CIN_NOTE_ON << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_NOTE_ON;
                            break;
                        case(MIDI_CIN_POLY_KEY_PRESS << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_POLY_KEY_PRESS;
                            break;
                        case(MIDI_CIN_CONTROL_CHANGE << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
                            break;
                        case(MIDI_CIN_PROGRAM_CHANGE << 4):
                            ExpectedDataBytes = 1;      //Expect 1 data bytes to follow
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PROGRAM_CHANGE;
                            break;
                        case(MIDI_CIN_CHANNEL_PREASURE << 4):
                            ExpectedDataBytes = 1;      //Expect 1 data bytes to follow
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CHANNEL_PREASURE;
                            break;
                        case(MIDI_CIN_PITCH_BEND_CHANGE << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTDeviceTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PITCH_BEND_CHANGE;
                            break;
                        default:
                            MIDIDeviceRxCommandState = STATE_WAIT_STATUS_BYTE;
                            ExpectedDataBytes = 0;
                            break;
                    }//switch(DeviceRxRunningStatusByte)                           
                }//else        
            }//if(DeviceRxRunningStatusByte & 0x80) 
        }//else if(MIDIDeviceRxCommandState == STATE_WAIT_STATUS_BYTE)
    }//if(UARTDeviceInByteCount > 0)
    
    // If the UART to USB translation has completed, then add packet to the OUT endpoint buffer(s)
    if(UARTMidiDevicePacketTranslated == true)
    {
        // For now will keep empty. It will be handled in APP_DeviceAudioMIDITasks()
    }// if(UARTMidiDevicePacketTranslated == TRUE)
}//void MIDI_host_InJackTasks    
 */

