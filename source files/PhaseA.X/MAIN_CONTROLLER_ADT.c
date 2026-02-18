/*
 * File:   MAIN_CONTROLLER_ADT.c
 * Author: waven
 *
 * Created on February 18, 2026, 11:50 AM
 */

#include <xc.h>
#include "MAIN_CONTROLLER_ADT.h"
#include "ADT_EUSART.h"

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
            } else {
                state = 0x00;
            }
            break;

        // STATE 1:
        // Turn ON the STATE_OK signal
        case 0x01:
            LATAbits.LATA3 = 1;   // Turn ON the STATE_OK signal
            break;

        default:
            // Optional: handle unexpected states
            state = 0x00;
            break;
    }
}