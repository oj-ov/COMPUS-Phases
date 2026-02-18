/*
 * File:   ADT_EUSART.c
 * Author: waven
 *
 * Created on February 18, 2026, 12:05 PM
 */

#include <xc.h>
#include "ADT_EUSART.h"

// Enabler bits: each bit corresponds to a different message
// Bit 0 = new day message
unsigned char enablers = 0x00;

// Confirmation bits: set when a message has been fully sent
unsigned char confirmedEnablers = 0x00;

// Message buffers
static unsigned char newDayMessage[33] = "> LSBank - New day has started!\n";
static unsigned char openExteriorDoorMessage[31] = "> LSBank - Open exterior door\n";

// Function prototypes (internal)
static void sendNewDayMessage(void);

unsigned char newMessageSent(void) {
    enablers |= 0x01;              // Enable "new day" message
    return (confirmedEnablers & 0x01);
}

unsigned char openExteriorDoorSent(void){
    enablers |= 0x02;
    return (confirmedEnablers & 0x02);
}

void eusartMotor(void) {

    static unsigned char state = 0x00;
    static unsigned char messageIndex = 0x00;

    switch (state) {

        // STATE 0:
        // Wait for any message to be enabled
        case 0x00:
            TXSTAbits.TXEN = 0; //Disable transmission
            if (enablers & 0x01) {
                state = 0x01;      // New day message
            }
            if (enablers & 0x02) {
                state = 0x02;      // Exterior door open message
            }
            break;

        // STATE 1:
        // Send the "new day" message character by character
        case 0x01:
            TXSTAbits.TXEN = 1; //Enable transmission
            if (messageIndex < 33) {

                if (TXSTAbits.TRMT == 1) {   // TXREG is ready, page 206
                    TXREG = newDayMessage[messageIndex++];
                }

            } else {
                // Message fully sent
                TXSTAbits.TXEN = 0;         //Disable transmission
                messageIndex = 0x00;
                enablers &= 0xFE;           // Disable bit 0
                confirmedEnablers |= 0x01;  // Confirm message sent
                state = 0x00;
            }
            break;

        // STATE 2:
        // Placeholder for future messages
        case 0x02:
            TXSTAbits.TXEN = 1; //Enable transmission
            if(messageIndex < 31){
                if(TXSTAbits.TRMT == 1){
                    TXREG = openExteriorDoorMessage[messageIndex++];
                }
            }else{
                // Message fully sent
                TXSTAbits.TXEN = 0;         //Disable transmission
                messageIndex = 0x00;
                enablers &= 0xFD;           // Disable bit 0
                confirmedEnablers |= 0x02;  // Confirm message sent
                state = 0x00;
            }
            break;

        default:
            state = 0x00;
            break;
    }
}
