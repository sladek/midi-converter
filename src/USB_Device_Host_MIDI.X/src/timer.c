#include <xc.h>

#include <stdbool.h>

#include "leds.h"
#include "timer.h"

void scan_state_flag();

void initTimer4_10ms()
{
    T4CON = 0x00; //Stops the Timer4 and reset control reg.
    TMR4 = 0x00; //Clear contents of the timer register
    PR4 = 624; //Load the Period register with the value 0xFFFF
    IPC6bits.T4IP = 0x01;   //Setup Timer4 interrupt for desired priority level
                            // (This example assigns level 1 priority)
    T4CONbits.TCKPS = 0x03; //Prescaler 1:256
    IFS1bits.T4IF = 0; //Clear the Timer4 interrupt status flag
    IEC1bits.T4IE = 1; //Enable Timer4 interrupts
    T4CONbits.TON = 1; //Start Timer4 with 
    //clock source set to the internal instruction cycle
}

void initTimer3_100ms()
{
    T3CON = 0x00;   //Stops the Timer3 and reset control reg.
    TMR3 = 0x00;    //Clear contents of the timer register
    PR3 = 6240;     //Load the Period register
    IPC2bits.T3IP = 0x01;   //Setup Timer3 interrupt for desired priority level
                            // (This example assigns level 1 priority)
    T3CONbits.TCKPS = 0x03; //Prescaler 1:256
    IFS0bits.T3IF = 0; //Clear the Timer3 interrupt status flag
    IEC0bits.T3IE = 1; //Enable Timer3 interrupts
    T3CONbits.TON = 1; //Start Timer3 with 
    //clock source set to the internal instruction cycle
}

void initTimer2_250ms()
{
    T2CON = 0x00;   //Stops the Timer2 and reset control reg.
    TMR2 = 0x00;    //Clear contents of the timer register
    PR2 = 15600;     //Load the Period register
    IPC1bits.T2IP = 0x01;   //Setup Timer2 interrupt for desired priority level
                            // (This example assigns level 1 priority)
    T2CONbits.TCKPS = 0x03; //Prescaler 1:256
    IFS0bits.T2IF = 0; //Clear the Timer2 interrupt status flag
    IEC0bits.T2IE = 1; //Enable Timer2 interrupts
    T2CONbits.TON = 1; //Start Timer2 with 
    //clock source set to the internal instruction cycle
}

void disableTimer2(){
    T2CONbits.TON = 0; //Stop Timer2 with     
    IEC0bits.T2IE = 0; //Disable Timer2 interrupts
}

/**
 * 
 */
void __attribute__((__interrupt__, __no_auto_psv__)) _T4Interrupt(void)
{
/* Interrupt Service Routine code goes here */

    _call10msEventHandler(); 
    IFS1bits.T4IF = 0; //Reset Timer4 interrupt flag and Return from ISR
}

void __attribute__((__interrupt__, __no_auto_psv__)) _T3Interrupt(void)
{
/* Interrupt Service Routine code goes here */

    _call100msEventHandler(); 
    IFS0bits.T3IF = 0; //Reset Timer3 interrupt flag and Return from ISR
}

void __attribute__((__interrupt__, __no_auto_psv__)) _T2Interrupt(void)
{
/* Interrupt Service Routine code goes here */

    _call250msEventHandler(); 
    IFS0bits.T2IF = 0; //Reset Timer3 interrupt flag and Return from ISR
}
