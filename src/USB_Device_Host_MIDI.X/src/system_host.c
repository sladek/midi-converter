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

#include <p24FJ64GB002.h>
#include "system_host.h"
#include "usb.h"
#include "leds.h"
#include "buttons.h"

/** CONFIGURATION Bits **********************************************/
// CONFIG4
#pragma config DSWDTPS = DSWDTPS3       // DSWDT Postscale Select (1:128 (132 ms))
#pragma config DSWDTOSC = LPRC          // Deep Sleep Watchdog Timer Oscillator Select (DSWDT uses Low Power RC Oscillator (LPRC))
#pragma config RTCOSC = SOSC            // RTCC Reference Oscillator Select (RTCC uses Secondary Oscillator (SOSC))
#pragma config DSBOREN = OFF            // Deep Sleep BOR Enable bit (BOR disabled in Deep Sleep)
#pragma config DSWDTEN = OFF            // Deep Sleep Watchdog Timer (DSWDT disabled)

// CONFIG3
#pragma config WPFP = WPFP0             // Write Protection Flash Page Segment Boundary (Page 0 (0x0))
#pragma config SOSCSEL = SOSC           // Secondary Oscillator Pin Mode Select (SOSC pins in Default (high drive-strength) Oscillator Mode)
#pragma config WUTSEL = LEG             // Voltage Regulator Wake-up Time Select (Default regulator start-up time used)
#pragma config WPDIS = WPDIS            // Segment Write Protection Disable (Segmented code protection disabled)
#pragma config WPCFG = WPCFGDIS         // Write Protect Configuration Page Select (Last page and Flash Configuration words are unprotected)
#pragma config WPEND = WPENDMEM         // Segment Write Protection End Page Select (Write Protect from WPFP to the last page of memory)

// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
// #pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary Oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Select bit (Use default SCL1/SDA1 pins for I2C1 )
#pragma config IOL1WAY = OFF            // IOLOCK One-Way Set Enable (The IOLOCK bit can be set and cleared using the unlock sequence)
#pragma config OSCIOFNC = ON            // OSCO Pin Configuration (OSCO pin functions as port I/O (RA3))
// #pragma config OSCIOFNC = OFF           // OSCO Pin Configuration (OSCO pin functions as clock output (CLKO))
#pragma config FCKSM = CSDCMD           // Clock Switching and Fail-Safe Clock Monitor (Sw Disabled, Mon Disabled)
#pragma config FNOSC = PRIPLL           // Initial Oscillator Select (Primary Oscillator with PLL module (XTPLL, HSPLL, ECPLL))
// #pragma config FNOSC = FRCPLL           // Initial Oscillator Select (Fast RC Oscillator with Postscaler and PLL module (FRCPLL))
#pragma config PLL96MHZ = ON            // 96MHz PLL Startup Select (96 MHz PLL Startup is enabled automatically on start-up)
// #pragma config PLLDIV = DIV2            // USB 96 MHz PLL Prescaler Select (Oscillator input divided by 2 (8 MHz input))
#pragma config PLLDIV = DIV4            // USB 96 MHz PLL Prescaler Select (Oscillator input divided by 4 (16 MHz input))
#pragma config IESO = OFF               // Internal External Switchover (IESO mode (Two-Speed Start-up) disabled)

// CONFIG1
#pragma config WDTPS = PS1              // Watchdog Timer Postscaler (1:1)
#pragma config FWPSA = PR32             // WDT Prescaler (Prescaler ratio of 1:32)
#pragma config WINDIS = OFF             // Windowed WDT (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC1/PGED1)
#pragma config GWRP = OFF               // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

void UART1Init();

/*********************************************************************
* Function: void SYSTEM_Initialize( SYSTEM_STATE state )
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
void SYSTEM_Initialize( SYSTEM_STATE state )
{
    //On the PIC24FJ64GB004 Family of USB micro-controllers, the PLL will not power up and be enabled
    //by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
    //This allows the device to power up at a lower initial operating frequency, which can be
    //advantageous when powered from a source which is not guaranteed to be adequate for 32MHz
    //operation.  On these devices, user firmware needs to manually set the CLKDIV<PLLEN> bit to
    //power up the PLL.
    {
        unsigned int pll_startup_counter = 600;
        CLKDIVbits.PLLEN = 1;
        while(pll_startup_counter--);
    }

   
    switch(state)
    {
        case SYSTEM_STATE_USB_HOST:
            UART1Init();    
            LED_Enable(LED_D1);
            break;
        default:
            break;
    }
}

/*********************************************************************
* Function: void UARTInit()
*
* Overview: Initializes the UART1.
*
* PreCondition: None
*
* Input:  None
*
* Output: None
*
********************************************************************/
void UART1Init()
{
    // UART2 Pin configuration
    // Configure Input Functions (Table 10-2)
    // Assign U1RX To Pin RP8 (Pin 17 on the DIP-28)
    RPINR19bits.U2RXR = 8;
    // Configure Output Functions (Table 10-3)
    // Assign U1TX (Function 3) to Pin RP7 (Pin 16 on the DIP-28)
    RPOR3bits.RP7R = 5;    
    // RB7 (U1TX) as output
    TRISBbits.TRISB7 = 0;

    // Value for 31250 baud rate.
    U2BRG = 30;
 
    U2MODEbits.UARTEN = 1; // UART1 is Enabled
    U2MODEbits.USIDL = 0; // Continue operation at Idlestate
    U2MODEbits.IREN = 0; // IrDA En/Decoder is disabled
    U2MODEbits.RTSMD = 0; // flow control mode
    U2MODEbits.UEN0=0; // Uart1 Tx and Rx pins are enabled and used.
    U2MODEbits.UEN1=0;
    U2MODEbits.WAKE = 0; // Wake-up disabled
    U2MODEbits.LPBACK = 0; // Loop-back is disabled
    U2MODEbits.ABAUD = 0; // auto baud is disabled
    U2MODEbits.RXINV = 0; // No RX inversion
    U2MODEbits.BRGH = 0; // low boud rate
    U2MODEbits.PDSEL = 0b00; // 8bit no parity
    U2MODEbits.STSEL = 0; // one stop bit
 

    U2STAbits.UTXISEL1 = 0; // Tx interrupt mode disabled
    U2STAbits.URXISEL0 = 0; // Interrupt when a last character is shifted out of Transmit Shift Register. All Tx operations are complete.
    U2STA &= 0xDFFF; // clear TXINV by bit masking
    U2STAbits.UTXBRK = 0; // sync break tx is disabled
    U2STAbits.UTXEN = 1; //transmit is enabled
    U2STAbits.URXISEL = 0b00; // interrupt flag bit is set when RXBUF is filled whith 1 character
    U2STAbits.ADDEN = 0; // address detect mode is disabled
 
    IFS1bits.U2RXIF = 0; // clear interrupt flag of rx
    IFS1bits.U2TXIF = 0; // clear interrupt flag of Tx
 
    IEC1bits.U2TXIE = 0 ; // Disabl uart transmit interrupt.
    IEC1bits.U2RXIE = 0 ; // Disable uart receive interrupt. 
}


void __attribute__((interrupt,auto_psv)) _USB1Interrupt()
{
        USB_HostInterruptHandler();

}

