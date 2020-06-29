#include <stdint.h>
#include <stdbool.h>

#include "system.h"
#include "midi_jack.h"
#include "usb_common.h"
#include "usb_host_midi.h"
#include "usb_device_midi.h"

//A circular buffer for storing received bytes from the host, out the UART to the attached device on the MIDI Out port
uint8_t        MIDIOutTxBuffer[MIDI_UART_BUFFER_SIZE];
uint8_t*       pTxBufReadLocation;
uint32_t*      pTxBufWriteLocation;        // This is a double word (4 bytes), since we will be writing a double word at a time
uint8_t        MIDIOutByteCount;

//A circular buffer for storing received bytes from the MIDI (UART), to be sent do attached USB MIDI device
uint8_t        MIDIInRxBuffer[MIDI_UART_BUFFER_SIZE];
uint8_t*       pRxUARTBufReadLocation;
uint8_t*       pRxUARTBufWriteLocation;
uint8_t        UARTInByteCount;

uint8_t        MIDIRxCommandState;
bool           SysExCommandPending;
bool           BytePending;
uint8_t        RxRunningStatusByte;
uint8_t        TxRunningStatusByte;

typedef enum
{
    STATE_WAIT_STATUS_BYTE,
    STATE_WAITING_BYTES
} STATE_WAIT;

typedef enum
{
    MIDI_CIN_1_BYTE_MESSAGE,
    MIDI_CIN_2_BYTE_MESSAGE,
    MIDI_CIN_3_BYTE_MESSAGE
} MIDI_CIN_N;

// MIDI packet used to translate MIDI UART to MIDI USB, with flag
USB_AUDIO_MIDI_PACKET UARTTranslatedToUSB;
bool        UARTmidiPacketTranslated;

// MIDI packet used to translate MIDI UART to MIDI USB for real time messages, with flag, and buffer
extern USB_AUDIO_MIDI_PACKET UARTRealTimeToUSB;
extern USB_AUDIO_MIDI_PACKET UARTRealTimeToUSBBuffer;
extern bool        UARTRealTimePacketTranslated;



void MIDI_Jack_Initialize()
{
        // Setup circular FIFO for MIDI Out port
    MIDIOutByteCount = 0;
    pTxBufReadLocation = &MIDIOutTxBuffer[0];
    pTxBufWriteLocation = (uint32_t*)&MIDIOutTxBuffer[0];
    
    // Setup circular FIFO for MIDI In port
    UARTInByteCount = 0;
    pRxUARTBufReadLocation = &MIDIInRxBuffer[0];
    pRxUARTBufWriteLocation = &MIDIInRxBuffer[0];

    // Clear UART translation packet and real-time packet/buffer
    UARTTranslatedToUSB.Val = 0;
    UARTRealTimeToUSB.Val = 0;
    UARTRealTimeToUSBBuffer.Val = 0;

}


void MIDI_U2RXInterruptHandler() 
{
    while (U2STAbits.URXDA) {

        *pRxUARTBufWriteLocation++ = U2RXREG;
        UARTInByteCount++;
        if((unsigned int)pRxUARTBufWriteLocation >= ((unsigned int)&MIDIInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
        {
            pRxUARTBufWriteLocation = &MIDIInRxBuffer[0];
        }
    }
    // Clear interrupt flag of rx
    IFS1bits.U2RXIF = 0;
}

//-----------------------------------------------------------------------------
//Check the MIDI UART FIFO buffer to see if any MIDI traffic was received by
// the interrupt. If so, we need to copy it from the UART FIFO buffer to the USB FIFO buffer,
// and translate it into the USB MIDI packet.
//-----------------------------------------------------------------------------
void MIDI_InJackTasks(void)
{
    static int8_t ExpectedDataBytes;
    static int8_t WriteIndex;
    
    //If we have something within the UART MIDI Rx Buffer
    if(UARTInByteCount > 0)
    {
        //Check if we received a new status (MIDI command) byte.  If so, regardless of previous state
        //jump to the state where we are starting to receive a new MIDI command packet.
        if(*pRxUARTBufReadLocation & 0x80) 
        {
            MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;
            RxRunningStatusByte = *pRxUARTBufReadLocation;   //Save the Status byte.
            
            //Reset write index pointer to to start of MIDI_x field of USB packet buffer,
            //unless we just recevied the MIDI_STATUS_SYSEX_END, where we will still need the old
            //WriteIndex value to know the proper CIN byte value of the next USB packet.
            if(*pRxUARTBufReadLocation != MIDI_STATUS_SYSEX_END)
            {
                WriteIndex = 2;
                UARTTranslatedToUSB.Val = 0;   //All pad bits/bytes = 0 according to MIDI-Audio USB specs.
            }// if(*pRxUARTBufReadLocation != MIDI_STATUS_SYSEX_END)
            
            //We are eventually going to consume the byte we just received, 
            //before returning in this scenario.
            pRxUARTBufReadLocation++;
            UARTInByteCount--;
            if((unsigned int)pRxUARTBufReadLocation >= ((unsigned int)&MIDIInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
            {
                pRxUARTBufReadLocation = &MIDIInRxBuffer[0];
            }
        }// if(*pRxUARTBufReadLocation & 0x80) 
        else if(MIDIRxCommandState == STATE_WAIT_STATUS_BYTE)   //Now check for "Running Status"
        {
            //If we get to here, this means we just received the first data byte,
            //that was part of a running status command.
            //In this case, we don't want to consume the ReceivedByte yet.  For this
            //iteration of this [if(UARTInByteCount > 0)] code, we just need to re-process 
            //the previous RxRunningStatusByte (ex: don't write to buffer here).  Then on the
            //next iteration we will actually consume the newest ReceivedByte.
        }// else if(MIDIRxCommandState == STATE_WAIT_STATUS_BYTE)
        else
        {
            //We are going to consume the (non-status) byte we just received, in this scenario.
        }// else
        
        //Check if we already received the Status (MIDI command) byte already, and
        //are currently waiting/expecting to receive data bytes.
        if(MIDIRxCommandState == STATE_WAITING_BYTES)
        {
            //Check if we are handling a SysEx command, or a normal command.
            if(SysExCommandPending == true)
            {
                //Handle the variable data length SysEx command.
                *((int8_t*)&UARTTranslatedToUSB + WriteIndex) = *pRxUARTBufReadLocation;
                pRxUARTBufReadLocation++;
                UARTInByteCount--;
                if((unsigned int)pRxUARTBufReadLocation >= ((unsigned int)&MIDIInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                {
                    pRxUARTBufReadLocation = &MIDIInRxBuffer[0];
                }
                if(WriteIndex < 3u)
                {
                    WriteIndex++;
                }// if(WriteIndex < 3u)
                else if(WriteIndex == 3)
                {
                    //We have accumulated a full USB-MIDI packet worth of data.
                    //Prepare state machine to send the packet.
                    UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_START; //Might also be continuing instead of starting.
                    
                    UARTmidiPacketTranslated = true; //Set boolean flag here to let USB code know it should prepare a packet.
                    WriteIndex = 1; //Move write pointer/index back to start of MIDI_x field of USB packet buffer
                }// else if(WriteIndex == 3)                 
            }// if(SysExCommandPending == TRUE)
            else
            {    
                *(int8_t*)(((int8_t*)&UARTTranslatedToUSB) + WriteIndex) = *pRxUARTBufReadLocation;
                pRxUARTBufReadLocation++;
                UARTInByteCount--;
                if((unsigned int)pRxUARTBufReadLocation >= ((unsigned int)&MIDIInRxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                {
                    pRxUARTBufReadLocation = &MIDIInRxBuffer[0];
                }
                if(ExpectedDataBytes != 0)
                {
                    WriteIndex++;
                    ExpectedDataBytes--;
                    //Check if we finished receiving all expected bytes in the MIDI command
                    if(ExpectedDataBytes == 0)
                    {
                        UARTmidiPacketTranslated = true; //Set boolean flag here so we know to add packet to buffer OUT
                        MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;                
                    }// if(ExpectedDataBytes == 0)
                }// if(ExpectedDataBytes != 0)
            }// else
        }// if(MIDIRxCommandState == STATE_WAITING_BYTES)
        else if(MIDIRxCommandState == STATE_WAIT_STATUS_BYTE)
        {
            ExpectedDataBytes = 0;  //Will get updated with correct value, once we have parsed the status byte and know how many data bytes to expect.
            MIDIRxCommandState = STATE_WAITING_BYTES;   //Initialize UART recevier state machine
            
            //We presumably have just received a status byte (MIDI command byte)
            if(RxRunningStatusByte & 0x80) //Check if we received a new status (MIDI command) byte, indicated by bit 7 of UART byte being set
            {
                //Check if it was a system common command of some kind (upper nibble = 0xF)
                if((RxRunningStatusByte & 0xF0) == 0xF0) 
                {
                    UARTTranslatedToUSB.MIDI_0 = RxRunningStatusByte;
                    //Check for system common commands
                    switch(RxRunningStatusByte)
                    {
                        case(MIDI_STATUS_SYSEX_START):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_START;
                            ExpectedDataBytes = 255;    //Not really expecting 255.  SysEx is variable/unknown length.
                            SysExCommandPending = true; //Let state machine know to perform special handling for this command.
                            break;
                        case(MIDI_STATUS_SYSEX_END):
                            //This case is special, since the actual data payload size is not fixed.
                            //However, when we get this byte, this means the SysEx command is completing,
                            //and therefore we can determine the residual number of bytes we need to send
                            //based on the WriteIndex variable.
                            if(WriteIndex == 1)
                                UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_ENDS_1;
                            else if(WriteIndex == 2)
                                UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_ENDS_2;
                            else if(WriteIndex == 3)
                                UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_SYSEX_ENDS_3;
                            //Complete the request here, rather than the normal handler.
                            WriteIndex = 2; //Move write pointer/index back to start of buffer for next MIDI command
                            UARTmidiPacketTranslated = true; //Set boolean flag here to let USB code know it should prepare a packet.
                            MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;  
                            SysExCommandPending = false;        //No longer handling a SysEx data flow.
                            ExpectedDataBytes = 0;
                            break;
                        case(MIDI_STATUS_MTC_QFRAME):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_2_BYTE_MESSAGE;
                            ExpectedDataBytes = 1;
                            break;
                        case(MIDI_STATUS_SONG_POSITION):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_3_BYTE_MESSAGE;
                            ExpectedDataBytes = 2;
                            break;
                        case(MIDI_STATUS_SONG_SELECT):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_2_BYTE_MESSAGE;
                            ExpectedDataBytes = 1;
                            break;
                        case(MIDI_STATUS_TUNE_REQUEST):
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_1_BYTE_MESSAGE;
                            ExpectedDataBytes = 0;
                            break;
                        default:
                            MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;
                            ExpectedDataBytes = 0;                            
                            break;
                    }//switch(RxRunningStatusByte)    
                    
                    //Check if we finished receiving all expected bytes in the MIDI command.
                    //This would be the case for system commands that consist only of the status byte. 
                    if((ExpectedDataBytes == 0) && (MIDIRxCommandState = STATE_WAITING_BYTES))  //Not note really waiting for extra bytes in this scenario.
                    {
                        //Prepare state machine to prepare USB transmit packet.
                        UARTmidiPacketTranslated = true; //Set boolean flag here to let USB code know it should prepare a packet.
                        MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;                
                    } 
                }//if((RxRunningStatusByte & 0xF0) == 0xF0) 
                else
                {
                    //Must have been a normal MIDI command status byte
                    UARTTranslatedToUSB.MIDI_0 = RxRunningStatusByte;
                    WriteIndex = 2;
                    RxRunningStatusByte &= 0xF0;   //Strip off the channel bits to get the command by itself
                    switch(RxRunningStatusByte)
                    {
                        case(MIDI_CIN_NOTE_OFF << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_NOTE_OFF;
                            break;
                        case(MIDI_CIN_NOTE_ON << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_NOTE_ON;
                            break;
                        case(MIDI_CIN_POLY_KEY_PRESS << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_POLY_KEY_PRESS;
                            break;
                        case(MIDI_CIN_CONTROL_CHANGE << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
                            break;
                        case(MIDI_CIN_PROGRAM_CHANGE << 4):
                            ExpectedDataBytes = 1;      //Expect 1 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PROGRAM_CHANGE;
                            break;
                        case(MIDI_CIN_CHANNEL_PREASURE << 4):
                            ExpectedDataBytes = 1;      //Expect 1 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CHANNEL_PREASURE;
                            break;
                        case(MIDI_CIN_PITCH_BEND_CHANGE << 4):
                            ExpectedDataBytes = 2;      //Expect 2 data bytes to follow
                            UARTTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PITCH_BEND_CHANGE;
                            break;
                        default:
                            MIDIRxCommandState = STATE_WAIT_STATUS_BYTE;
                            ExpectedDataBytes = 0;
                            break;
                    }//switch(RxRunningStatusByte)                           
                }//else        
            }//if(RxRunningStatusByte & 0x80) 
        }//else if(MIDIRxCommandState == STATE_WAIT_STATUS_BYTE)
    }//if(UARTInByteCount > 0)
    

 /* this will be handled separately in device host app. But for now I will keep it here 

    // If the UART to USB translation has completed, then add packet to the OUT endpoint buffer(s)
    if(UARTmidiPacketTranslated == true)
    {
        int8_t currentEndpoint;
        
        // Clear the flag
        UARTmidiPacketTranslated = false;
        
        // Loop through the endpoints
        for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
        {
            // If this is an OUT endpoint
            if(USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint) == OUT)
            {
                // Then add the translated MIDI packet to its buffer and increment (we're only doing one MIDI packet per USB packet)
                endpointBuffers[currentEndpoint].pBufWriteLocation->Val = UARTTranslatedToUSB.Val;
                endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                
                // Check to see if we've gotten to the end of the USB buffer array
                if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart
                >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                {
                    // If so, then loop it back to the beginning of the array
                    endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                }
                
                // If we want to send the data to just one endpoint, then we'll break, otherwise we'll spam all of the OUT endpoints
                #ifdef SEND_TO_ONE_EP
                    break;
                #endif
            }// if(USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint) == OUT)
        }// for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)    
    }// if(UARTmidiPacketTranslated == TRUE)
  */

}//void MIDI_host_InJackTasks    
    
    
