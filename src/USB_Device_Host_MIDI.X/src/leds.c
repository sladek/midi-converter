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
*******************************************************************************/

#include <xc.h>

#include <stdbool.h>

#include "leds.h"

/* On MIcrostick II, there is only i LED D6, so I defined it here as LED_D1. */
#define LED_D1_LAT      LATAbits.LATA0     //Overlaps with S3
#define LED_D1_TRIS     TRISAbits.TRISA0

#define LED_D2_LAT      LATAbits.LATA1     
#define LED_D2_TRIS     TRISAbits.TRISA1

#define LED_ON  1
#define LED_OFF 0

#define PIN_INPUT   1
#define PIN_OUTPUT  0



static LED_STATE LED1_State = LED_STATUS_OFF;
static LED_STATE LED2_State = LED_STATUS_BLINK;

/*********************************************************************
* Function: void LED_On(LED led);
*
* Overview: Turns requested LED on
*
* PreCondition: LED configured via LED_Configure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_On(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_On(LED led)
{
    switch(led)
    {
        case LED_D1:
            LED_D1_LAT = LED_ON;
            break;
        case LED_D2:
            LED_D2_LAT = LED_ON;
            break;

        default:
            break;
    }
}

/*********************************************************************
* Function: void LED_Off(LED led);
*
* Overview: Turns requested LED off
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Off(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Off(LED led)
{
    switch(led)
    {

        case LED_D1:
            LED_D1_LAT = LED_OFF;
            break;
        case LED_D2:
            LED_D2_LAT = LED_OFF;
            break;

        default:
            break;
    }
}

/*********************************************************************
* Function: void LED_Toggle(LED led);
*
* Overview: Toggles the state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Toggle(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Toggle(LED led)
{
    switch(led)
    {

        case LED_D1:
            LED_D1_LAT ^= 1;
            break;
            
        case LED_D2:
            LED_D2_LAT ^= 1;
            break;

        default:
            break;
    }
}

/*********************************************************************
* Function: bool LED_Get(LED led);
*
* Overview: Returns the current state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Get(LED_CONNECTION_DETECTED);
*
* Output: true if on, false if off
*
********************************************************************/
bool LED_Get(LED led)
{
    switch(led)
    {
        
        case LED_D1:
            return ( (LED_D1_LAT == LED_ON) ? true : false );

        case LED_D2:
            return ( (LED_D2_LAT == LED_ON) ? true : false );

        default:
            break;
    }
    return false;
}

/*********************************************************************
* Function: void LED_Enable(LED led);
*
* Overview: Configures the LED for use by the other LED API
*
* PreCondition: none
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*
* Output: none
*
********************************************************************/
void LED_Enable(LED led)
{
    switch(led)
    {

        case LED_D1:
            LED_D1_TRIS = PIN_OUTPUT;
            break;

        case LED_D2:
            LED_D2_TRIS = PIN_OUTPUT;
            break;

        default:
            break;
    }
}

/********************************************************************
 * Function: void set_LED1_state(LED_STATE state) 
 *
 * Overview: Sets status of LED1 state-machine;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED1_Set_State(LED_STATE state){
    LED1_State = state;
}

/********************************************************************
 * Function: void led1_on() 
 *
 * Overview: Sets status of LED1 state-machine to LED_STATUS_ON;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED1_On(){
    LED1_Set_State(LED_STATUS_ON);
}

/********************************************************************
 * Function: void led1_off() 
 *
 * Overview: Sets status of LED1 state-machine to LED_STATUS_OFF;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED1_Off(){
    LED1_Set_State(LED_STATUS_OFF);
}

/********************************************************************
 * Function: void blink_LED1(void);
 * 
 * Overview: Make LED1 blink;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED1_Blink(void){
    LED1_Set_State(LED_STATUS_BLINK);
}

/********************************************************************
 * Function: void blink_LED1_once(void);
 * 
 * Overview: Make LED1 blink once;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED1_Blink_Once(void){
    LED1_Set_State(LED_STATUS_BLINK_ONCE);
}

/********************************************************************
 * Function: void set_LED2_state(LED_STATE state) 
 *
 * Overview: Sets status of LED2 state-machine;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED2_Set_State(LED_STATE state){
    LED2_State = state;
}

/********************************************************************
 * Function: void led2_on() 
 *
 * Overview: Sets status of LED2 state-machine to LED_STATUS_ON;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED2_On(){
    LED2_Set_State(LED_STATUS_ON);
}

/********************************************************************
 * Function: void led2_off() 
 *
 * Overview: Sets status of LED2 state-machine to LED_STATUS_OFF;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED2_Off(){
    LED2_Set_State(LED_STATUS_OFF);
}

/********************************************************************
 * Function: void blink_LED2(void);
 * 
 * Overview: Make LED2 blink;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED2_Blink(void){
    LED2_Set_State(LED_STATUS_BLINK);
}

/********************************************************************
 * Function: void blink_LED2_once(void);
 * 
 * Overview: Make LED2 blink once;
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output: none
 * 
 */
void LED2_Blink_Once(void){
    LED2_Set_State(LED_STATUS_BLINK_ONCE);
}



void LED1_handler(){
    LED1_StateMachine();
}

void LED2_handler(){
    LED2_StateMachine();
}

/*******************************************************************
 * Function: void LED1_StateMachine();
 * 
 * Overview: Changes status of LED1 based on state-machine
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output none
 * 
 *******************************************************************/
void LED1_StateMachine(){
    static unsigned int led_count=0;
    static LED_STATE current_state = LED_STATUS_OFF;

//    const int c_led_count = 10;
//    if(led_count == 0)
//        led_count = c_led_count;
//    led_count--;

    switch (LED1_State){
        case LED_STATUS_ON:
            if (current_state != LED_STATUS_ON){
                current_state = LED_STATUS_ON;
                LED_On(LED_D1);
            }
            break;
        case LED_STATUS_OFF:
            if (current_state != LED_STATUS_OFF){
                current_state = LED_STATUS_OFF;
                LED_Off(LED_D1);
            }
            break;
        case LED_STATUS_BLINK:
            if (current_state != LED_STATUS_BLINK){
                current_state = LED_STATUS_BLINK;
//                led_count = c_led_count;
                LED_Toggle(LED_D1);
                break;
            }
            if (led_count == 0){
                LED_Toggle(LED_D1);
            }
            break;
        case LED_STATUS_BLINK_ONCE:
            if (current_state != LED_STATUS_BLINK_ONCE){
                current_state = LED_STATUS_BLINK_ONCE;
//                led_count = c_led_count;
                LED_On(LED_D1);
                break;
            }
            if (led_count == 0){
                LED1_State = current_state = LED_STATUS_OFF;
                LED_Off(LED_D1);
            }
            break;
    }
            
}

/*******************************************************************
 * Function: void LED2_StateMachine();
 * 
 * Overview: Changes status of LED2 based on state-machine
 * 
 * Precondition: none
 * 
 * Input: none
 * 
 * Output none
 * 
 *******************************************************************/
void LED2_StateMachine(){
    static unsigned int led_count=0;
    static LED_STATE current_state = LED_STATUS_OFF;

//    const int c_led_count = 10;
//    if(led_count == 0)
//        led_count = c_led_count;
//    led_count--;

    switch (LED2_State){
        case LED_STATUS_ON:
            if (current_state != LED_STATUS_ON){
                current_state = LED_STATUS_ON;
                LED_On(LED_D2);
            }
            break;
        case LED_STATUS_OFF:
            if (current_state != LED_STATUS_OFF){
                current_state = LED_STATUS_OFF;
                LED_Off(LED_D2);
            }
            break;
        case LED_STATUS_BLINK:
            if (current_state != LED_STATUS_BLINK){
                current_state = LED_STATUS_BLINK;
//                led_count = c_led_count;
                LED_Toggle(LED_D2);
                break;
            }
            if (led_count == 0){
                LED_Toggle(LED_D2);
            }
            break;
        case LED_STATUS_BLINK_ONCE:
            if (current_state != LED_STATUS_BLINK_ONCE){
                current_state = LED_STATUS_BLINK_ONCE;
//                led_count = c_led_count;
                LED_On(LED_D2);
                break;
            }
            if (led_count == 0){
                LED2_State = current_state = LED_STATUS_OFF;
                LED_Off(LED_D2);
            }
            break;
    }
            
}