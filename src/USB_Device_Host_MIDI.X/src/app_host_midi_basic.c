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
#include <stdlib.h>
#include "system.h"
#include "usb.h"
#include "usb_host_midi.h"
#include "usb_host_local.h"
#include "usb_common.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// *****************************************************************************
// *****************************************************************************
// Internal Function Prototypes
// *****************************************************************************
// *****************************************************************************
void DBG_LED();
//void MIDI_Host_OutJackTasks(void);
void MIDI_InJackTasks(void);

// *****************************************************************************
// *****************************************************************************
// Macros
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Constants
// *****************************************************************************
// *****************************************************************************

// Application specific
#define MIDI_USB_BUFFER_SIZE                (uint8_t)4
#define NUM_MIDI_PKTS_IN_USB_PKT            (uint8_t)1

// *****************************************************************************
// *****************************************************************************
// Type definitions
// *****************************************************************************
// *****************************************************************************
typedef enum _APP_STATE
{
    DEVICE_NOT_CONNECTED,
    DEVICE_READY,
    ERROR_REPORTED      /* need to add application states for data interface */
} APP_STATE;

typedef enum
{
    TX_DATA = 0,
    TX_DATA_WAIT,
    RX_DATA,
    RX_DATA_WAIT,
    TX_REAL_TIME_DATA,
    TX_REAL_TIME_DATA_WAIT
} TX_RX_STATE;

typedef enum
{
    STATE_WAIT_STATUS_BYTE,
    STATE_WAITING_BYTES
} STATE_WAIT;

typedef struct
{
    TX_RX_STATE             TransferState;      // The transfer state of the endpoint
    uint8_t                 numOfMIDIPackets;   // Each USB Packet sent from a device has the possibility of holding more than one MIDI packet,
                                                //  so this is to keep track of how many MIDI packets are within a USB packet (between 1 and 16, or 4 and 64 bytes)
    USB_AUDIO_MIDI_PACKET*  bufferStart;        // The 2D buffer for the endpoint. There are MIDI_USB_BUFFER_SIZE USB buffers that are filled with numOfMIDIPackets
                                                //  MIDI packets. This allows for MIDI_USB_BUFFER_SIZE USB packets to be saved, with a possibility of up to 
                                                //  numOfMIDIPackets MID packets within each USB packet.
    USB_AUDIO_MIDI_PACKET*  pBufReadLocation;   // Pointer to USB packet that is being read from
    USB_AUDIO_MIDI_PACKET*  pBufWriteLocation;  // Pointer to USB packet that is being written to
}ENDPOINT_BUFFER;


// *****************************************************************************
// *****************************************************************************
// Local Variables
// *****************************************************************************
// *****************************************************************************
volatile APP_STATE  midiState;

MIDI_DEVICE *deviceHandle;

bool buttonReleased;

// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************
ENDPOINT_BUFFER*    endpointBuffers;    // A pointer to a dynamically allocated array that contains a buffer for each endpoint

// MIDI packet used to translate MIDI UART to MIDI USB for real time messages, with flag, and buffer
USB_AUDIO_MIDI_PACKET   UARTRealTimeToUSB;
USB_AUDIO_MIDI_PACKET   UARTRealTimeToUSBBuffer;
bool                    UARTRealTimePacketTranslated;

bool        somethingToSend;

uint16_t        NumGets;
uint16_t        NumSends;



/*********************************************************************
* Function: void APP_HostMIDIBasicInitialize(void);
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
void APP_HostMIDIBasicInitialize()
{
    midiState = DEVICE_NOT_CONNECTED;
    
    // Set device handle to NULL
    deviceHandle = NULL;
    
    UARTRealTimePacketTranslated = false;

    NumGets = 0;
    NumSends = 0;

}

/*********************************************************************
* Function: void APP_HostMIDIBasicTasks(void);
*
* Overview: Keeps the demo running.
*
* PreCondition: The demo should have been initialized via
*   the APP_HostMIDIBasicInitialize()
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_HostMIDIBasicTasks()
{
    uint8_t numOfBytesRcvd;
    uint8_t currentEndpoint;

    numOfBytesRcvd = 0;
    
    switch(midiState)
    {
        case DEVICE_NOT_CONNECTED:
            break;
        case DEVICE_READY:
            //-----------------------------------------------------------------------------
            //Check if we received any RX data from the MIDI device attached (MIDI In port)
            //-----------------------------------------------------------------------------
            MIDI_InJackTasks();

            //-----------------------------------------------------------------------------
            //Check if we received any RX data from the MIDI device attached (MIDI Out port)
            //-----------------------------------------------------------------------------
//            MIDI_Host_OutJackTasks();
 
            for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
            {
                switch(endpointBuffers[currentEndpoint].TransferState)
                {
                    /** Send IN Packet: Loops here until successfuly sends packet **/
                    case RX_DATA:
                        // Then clear current endpoint's buffer, and try to read
                        memset(endpointBuffers[currentEndpoint].pBufWriteLocation, 0x00, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET));
                        if(USBHostMIDIRead(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufWriteLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET) ) == USB_SUCCESS )
                        {
                            // If successful, then update the transfer state to wait for the transaction to complete    

                            endpointBuffers[currentEndpoint].TransferState = RX_DATA_WAIT;
                        }
                        break;

                    /** Send OUT Packet: Loops here until successfuly sends packet **/
                    case TX_DATA:
                        // See if there is a real-time message to send. These have a higher priority
                        if(UARTRealTimePacketTranslated == true)
                        {
                            // If so, then clear the flag and write the buffer to the transmit packet
                            UARTRealTimeToUSB.Val = UARTRealTimeToUSBBuffer.Val;
                            UARTRealTimePacketTranslated = true;
                            
                            // And change state to that case, so next time it will get transmitted
                            endpointBuffers[currentEndpoint].TransferState = TX_REAL_TIME_DATA;
                        }
                            
                        // Otherwise, try to send if there's something in the buffer
                        else if(endpointBuffers[currentEndpoint].pBufReadLocation != endpointBuffers[currentEndpoint].pBufWriteLocation)
                        {
                            if(USBHostMIDIWrite(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufReadLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET)) == USB_SUCCESS)
                            {
                                // Update the transfer state to wait for the transaction to complete    
                                endpointBuffers[currentEndpoint].TransferState = TX_DATA_WAIT;
                            }
                        }    
                        break;
                        
                    case TX_REAL_TIME_DATA:
                        // Try to send the real-time data
                        if(USBHostMIDIWrite(deviceHandle, currentEndpoint, &UARTRealTimeToUSB, sizeof(USB_AUDIO_MIDI_PACKET)) == USB_SUCCESS)
                        {
                            endpointBuffers[currentEndpoint].TransferState = TX_REAL_TIME_DATA_WAIT;
                        }
                        break;
                    
                    /** Get IN Data: Loops here for this endpoint until successfully receives data packet **/
                    case RX_DATA_WAIT:
                        // If the transfer is complete
                        if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                        {
                            // Update the transfer state to try and receive another packet
                            endpointBuffers[currentEndpoint].TransferState = RX_DATA;
                            
                            // Then increment the write location pointer to the next USB buffer
                            endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                            
                            // Check to see if we've gotten to the end of the USB buffer array
                            if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart
                            >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                            {
                                // If so, then loop it back to the beginning of the array
                                endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                            }
                        }
                        break;
                    
                    /** Send OUT Data: Loops here for this endpoint until successfully sends data packet **/
                    case TX_DATA_WAIT:
                        // If the transfer is complete
                        if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                        {
                            // Update the transfer state to try and send another packet
                            endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                            
                            // Then increment the read location pointer to the next USB buffer
                            endpointBuffers[currentEndpoint].pBufReadLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                                
                            // Check to see if we've gotten to the end of the USB buffer array
                            if(endpointBuffers[currentEndpoint].pBufReadLocation - endpointBuffers[currentEndpoint].bufferStart
                            >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                            {
                                // If so, then loop it back to the beginning of the array
                                endpointBuffers[currentEndpoint].pBufReadLocation = endpointBuffers[currentEndpoint].bufferStart;
                            }
                        }        
                        break;
                        
                    case TX_REAL_TIME_DATA_WAIT:
                        // If the transfer is complete
                        if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                        {
                            // Update the transfer state back to trying to send another packet
                            endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                        }
                        break;    
                }
            }    
        default:
            break;
    }

}


/*********************************************************************
* Function: bool APP_HostMIDIBasicEventHandler( uint8_t address, USB_EVENT event, void *data, uint32_t size );
*
* Overview: Handles USB events for the USB host MIDI demo
*
* PreCondition: None
*
* Input: uint8_t - address of the device causing the event
*        USB_EVENT - the event that occurred
*        void* - data associated with the event
*        uint32_t - size of the data for the event
*
* Output: None
*
********************************************************************/
bool APP_HostMIDIBasicEventHandler( uint8_t address, USB_EVENT event, void *data, uint32_t size )
{

    uint8_t currentEndpoint;
    ENDPOINT_DIRECTION direction;
    
    switch((int)event)
    {
        case EVENT_MIDI_ATTACH:

            deviceHandle = data;
            midiState = DEVICE_READY;
            
            endpointBuffers = malloc( sizeof(ENDPOINT_BUFFER) * USBHostMIDINumberOfEndpoints(deviceHandle) );
            
            for( currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++ )
            {
                direction = USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint);
                // For OUT endpoints
                if(direction == OUT)
                {   
                    // We only want to send NUM_MIDI_PKTS_IN_USB_PKT MIDI packet per USB packet
                    endpointBuffers[currentEndpoint].numOfMIDIPackets = NUM_MIDI_PKTS_IN_USB_PKT;
                    
                    // And we want to start it off transmitting data
                    endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                }
                // For IN endpoints
                else if (direction == IN)
                {
                    // We will accept however many will fit inside the maximum USB packet size
                    endpointBuffers[currentEndpoint].numOfMIDIPackets = USBHostMIDISizeOfEndpoint(deviceHandle, currentEndpoint) / sizeof(USB_AUDIO_MIDI_PACKET);
                    
                    // And we want to start it off trying to read data
                    endpointBuffers[currentEndpoint].TransferState = RX_DATA;
                }
                else
                {
                    continue;
                }
                
                // Allocate the 2D buffer, and keep track of the write and read locations
                endpointBuffers[currentEndpoint].bufferStart = malloc( sizeof(USB_AUDIO_MIDI_PACKET) * endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE );
                endpointBuffers[currentEndpoint].pBufReadLocation = endpointBuffers[currentEndpoint].bufferStart;
                endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
            }    
            
            // Testing Purposes
//            endpointOne = &endpointBuffers[0];
//            endpointTwo = endpointBuffers+1;
            
            //UART2PrintString( "MIDI demo device attached - event, deviceAddress=" );
            //UART2PutDec( address );
            //UART2PrintString( "\r\n" );
            //UART2PrintString( "\r\n" );

            return true;
        case EVENT_MIDI_DETACH:
            for( currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++ )
            {
                free(endpointBuffers[currentEndpoint].bufferStart);
                endpointBuffers[currentEndpoint].bufferStart = NULL;
                endpointBuffers[currentEndpoint].pBufReadLocation = NULL;
                endpointBuffers[currentEndpoint].pBufWriteLocation = NULL;
            }
            
            free(endpointBuffers);
            endpointBuffers = NULL;
            
            deviceHandle = NULL;
            midiState = DEVICE_NOT_CONNECTED;
            return true;
            
            
        case EVENT_MIDI_TRANSFER_DONE:   // The main state machine will poll the driver.
            break;
        case EVENT_TRANSFER:
        case EVENT_VBUS_REQUEST_POWER:
        case EVENT_VBUS_RELEASE_POWER:
        case EVENT_HUB_ATTACH:
        case EVENT_UNSUPPORTED_DEVICE:
        case EVENT_CANNOT_ENUMERATE:
        case EVENT_CLIENT_INIT_ERROR:
        case EVENT_OUT_OF_MEMORY:
        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
        case EVENT_SUSPEND:
        case EVENT_DETACH:
        case EVENT_RESUME:
        case EVENT_BUS_ERROR:
            return true;
            break;

        default:
            break;
    }

    return false;
}


//-----------------------------------------------------------------------------
//Check if any valid data is currently waiting in the endpoint buffers that needs
// to be sent over the UART.
//-----------------------------------------------------------------------------
/*
void MIDI_Host_OutJackTasks(void)
{
    int8_t currentEndpoint;
    
    // First, go through each IN endpoint and add packets from its USB buffer to the UART OUT FIFO
    for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
    {
        // Check to see if this is an IN endpoint, and if the buffer has any packets to be read
        if((USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint) == IN)
        && (endpointBuffers[currentEndpoint].pBufReadLocation != endpointBuffers[currentEndpoint].pBufWriteLocation))
        {
            int8_t midiPkt;            
                
            // If so, then parse through the entire USB packet for each individual MIDI packet
            for(midiPkt = 0; midiPkt < endpointBuffers[currentEndpoint].numOfMIDIPackets; midiPkt++)
            {
                if(endpointBuffers[currentEndpoint].pBufReadLocation->Val == 0ul)
                {
                    // If there's nothing in this MIDI packet, then skip the rest of the USB packet
                    endpointBuffers[currentEndpoint].pBufReadLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets - midiPkt;
                    break;
                }    
                else
                {
                    // Otherwise add it to the UART OUT FIFO and increment both pointers
                    *pTxBufWriteLocation = endpointBuffers[currentEndpoint].pBufReadLocation->Val;
                    pTxBufWriteLocation++;
                    endpointBuffers[currentEndpoint].pBufReadLocation++;
                    MIDIOutByteCount += 4;
                    
                    // Check to see if we've gotten to the end of the UART OUT FIFO array
                    if((unsigned int)pTxBufWriteLocation >= ((unsigned int)&MIDIOutTxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                    {
                        // If so, then loop it back to the beginning of the array
                        pTxBufWriteLocation = (uint32_t*)&MIDIOutTxBuffer[0];
                    }
                }
            }
            
            // After parsing through entire USB packet, check to see if we've gotten to the end of the USB array
            if(endpointBuffers[currentEndpoint].pBufReadLocation - endpointBuffers[currentEndpoint].bufferStart
            >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
            {
                // If so, then loop it back to the beginning of the array
                endpointBuffers[currentEndpoint].pBufReadLocation = endpointBuffers[currentEndpoint].bufferStart;
            }
        }
    }        
    
    
    // Next, continue sending UART data from the FIFO UART OUT buffer
    if((MIDIOutByteCount != 0u) && (U2STAbits.TRMT == 1)) 
    {
        //Check if the byte was not on a 4-byte boundary.  If so, that means we
        //are currently pointing to a Cable#/CIN byte, which we shouldn't 
        //re-transmit over the UART MIDI output port.  Just gobble it up/throw
        //it away, since we are currently only implementing support for one MIDI
        //OUT jack anyway.
        //All other MIDI status/data bytes we re-transmit directly.
        switch((unsigned int)(pTxBufReadLocation - &MIDIOutTxBuffer[0]) & 0x0003)
        {
            case 0:
                //In this case, we are currently pointing to the Cable#/CIN byte.
                //Just gobble it up from the FIFO and perform error case checking.
                pTxBufReadLocation++;
                //Check if time for read FIFO pointer wrap around
                if(pTxBufReadLocation >= (&MIDIOutTxBuffer[0] + MIDI_UART_BUFFER_SIZE))
                {
                    pTxBufReadLocation = &MIDIOutTxBuffer[0];
                }    
                MIDIOutByteCount--; //Consumed a byte from the circular FIFO
                
                //Now error check, to make sure that the byte we are currently pointing
                //a is a status byte (bit #7 set).  It always should be, unless the
                //host sends an illegal MIDI data packet (not 32-bit fixed width
                //like specifications require).  
                if(!(*pTxBufReadLocation & 0x80))
                {
                    //In this scenario, we should flush the FIFO and re-inilize our
                    //pointers.  This is draconian, but the data in the buffer
                    //probably isn't trustworthy.  Additionally, during operation 
                    //the FIFO wouldn't normally contain more than 1 MIDI command 
                    //at a time anyway, so this shouldn't result in any major loss
                    //of data.
                    pTxBufWriteLocation = (uint32_t*)&MIDIOutTxBuffer[0];
                    pTxBufReadLocation = &MIDIOutTxBuffer[0];
                    MIDIOutByteCount = 0;
                }
                break;
            case 1:
                if(*pTxBufReadLocation == TxRunningStatusByte)
                {
                    pTxBufReadLocation++;
                    MIDIOutByteCount--;
                    break;
                }   
                else if(*pTxBufReadLocation < 0xF8u)
                {
                    TxRunningStatusByte = *pTxBufReadLocation;
                }
            default:  
            case 2:
            case 3:
                U2TXREG = *pTxBufReadLocation;
                pTxBufReadLocation++;
                //Check if time for read FIFO pointer wrap around
                if((unsigned int)pTxBufReadLocation >= ((unsigned int)&MIDIOutTxBuffer[0] + (unsigned int)MIDI_UART_BUFFER_SIZE))
                {
                    pTxBufReadLocation = &MIDIOutTxBuffer[0];
                }    
                MIDIOutByteCount--; //Consumed a byte from the circular FIFO
                break;
        }//switch((unsigned int)(pTxBufReadLocation - &MIDIOutTxBuffer[0]) & 0x0003)
    }//if((MIDIOutByteCount != 0u) && (PIR3bits.TX2IF == 1)) 
}//void MIDITasks(void)

 */
