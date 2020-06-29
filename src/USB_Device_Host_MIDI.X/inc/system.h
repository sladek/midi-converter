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

#ifndef SYSTEM_H
#define SYSTEM_H

#include <p24FJ64GB002.h>
#include <stdbool.h>

#include "buttons.h"
#include "leds.h"

#include "io_mapping.h"
#include "power.h"
#include "usb_device_config.h"

#define MAIN_RETURN int

/*** Scan states ****************************************************/
typedef enum
{
    SCAN_STATE_DEVICE,
    SCAN_STATE_HOST
} SCAN_STATE;
/*********************************************************************

/*** System States **************************************************/
typedef enum
{
    SYSTEM_STATE_USB_START,
    SYSTEM_STATE_USB_SUSPEND,
    SYSTEM_STATE_USB_RESUME
} SYSTEM_STATE;

/*********************************************************************
* Function: void SYSTEM_HOST_Initialize( SYSTEM_HOST_STATE state )
*
* Overview: Initializes the system.
*
* PreCondition: None
*
* Input:  SYSTEM_STATE - the state to initialize the system into
*
* Output: None
*
********************************************************************/
void SYSTEM_Initialize( SYSTEM_STATE state );


/**
 * Function: void MIDI_RXInterruptHandler()
 * 
 * Overview: MIDI serial port interrupt handler
 * 
 * Precondition: None
 * 
 * Input: None
 * 
 * Output: None
 * 
 */
void MIDI_RXInterruptHandler();

/*********************************************************************
* Function: void SYSTEM_DEVICE_Tasks(void)
*
* Overview: Runs system level tasks that keep the system running
*
* PreCondition: System has been initalized with SYSTEM_Initialize()
*
* Input: None
*
* Output: None
*
********************************************************************/
//void SYSTEM_Tasks(void);
#define SYSTEM_Tasks()

#endif //SYSTEM_H
