/*
 * File:   MAIN_CONTROLLER_ADT.c
 * Author: waven
 *
 * Created on February 18, 2026, 11:50 AM
 */

#include <xc.h>
#include "MAIN_CONTROLLER_ADT.h"
#include "ADT_EUSART.h"
#include "ADT_TIMER.h"

#define TWO_SECONDS 1000

unsigned char timer0;

void MainControllerMotor(void) {

    static unsigned char state = 0x00;

    switch (state) {

        // STATE 0:
        // Send the "new day" message to the PC screen
        case 0x00:
            /* Calling the EUSART ADT to enable its message being sent and then
             * waiting for it to be sent before changing the state
             */
            if (newMessageSent() == 0x01) {
                state = 0x01;
            }
            break;

        // STATE 1:
        // Turn ON the STATE_OK signal
        case 0x01:
            LATAbits.LATA3 = 1;   // Turn ON the STATE_OK signal
            if(LATDbits.LATD7 == 0 ){ // 1: No magnet. 2: Magnet.
                state = 0x02;
            }
            break;
            
            
        // STATE 2: 
        // Send the open exterior door message
        case 0x02:
            if(openExteriorDoorSent() == 0x02){
                state = 0x03;
            }
            break;
        
        // STATE 3: 
        // Create new timer to count new seconds and reset its ticks
        case 0x03:
            TI_NewTimer(&timer0);
            TI_ResetTics(timer0);
            state = 0x04;
            break;
            
        // STATE 4: 
        // Start counting the ticks for 2 seconds
        case 0x04:
            if(TI_GetTics(timer0) == TWO_SECONDS){
                TI_ResetTics(timer0);
                state = 0x05;
            }
            break;

        default:
            // Optional: handle unexpected states
            state = 0x00;
            break;
    }
}