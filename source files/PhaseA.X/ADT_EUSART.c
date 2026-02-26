#include <xc.h>
#include "ADT_EUSART.h"
/*
an idea for u to work on.
enablers variable valurestorage
bit 0 (0x01): new day
bit 1 (0x02): open exterior
bit 2 (0x04): close exterior
bit 3 (0x08): enter PIN
bit 4 (0x10): permission denied
bit 5 (0x20): open interior
bit 6 (0x40): close interior
bit 7 (0x80): exit requested
enablers2 variable value storage
bit 0 (0x01): open both doors
bit 1 (0x02): close both doors
bit 2 (0x04): thief intercepted
bit 3 (0x08): reset system
*/
// Enabler bits: each bit corresponds to a different message
// Bit 0 = new day message
unsigned char enablers = 0x00;
unsigned char confirmed1 = 0x00;
unsigned char enablers2 = 0x00;
unsigned char confirmed2 = 0x00; 
// Confirmation bits: set when a message has been fully sent
unsigned char confirmedEnablers = 0x00;

// Message buffers
static const char newDayMessage[ ] = "> LSBank - New day has started!\n";
static const char openExteriorDoorMessage[ ] = "> LSBank - Open exterior door\n";
static const char closeExteriorDoorMessage[ ] = "> LSBank - Close exterior door\n";
static const char enterPinMessage[ ] = "> LSBank - Enter PIN\n";
static const char permissionDeniedMessage[ ] = "> LSBank - Permission denied\n";

static const char openInteriorDoorMessage[ ] = "> LSBank - Open interior door\n";
static const char closeInteriorDoorMessage[ ] = "> LSBank - Close interior door\n";
static const char openBothDoorMessage[ ] = "> LSBank - Open both doors\n";
static const char closeBothDoorMessage[ ] = "> LSBank - Close both doors\n";


static const char thiefMessage[ ] = "> LSBank - Thief Intercepted\n";
static const char resetMessage[ ] = "> LSBank - Reset System: \n";

// Function prototypes (internal)
static void sendNewDayMessage(void);

unsigned char newMessageSent(void) {
    confirmedEnablers &= ~0x01;
    enablers |= 0x01;              // Enable "new day" message // so the or operation
    return (confirmedEnablers & 0x01);
}

unsigned char openExteriorDoorSent(void){
    enablers |= 0x02;
    return (confirmedEnablers & 0x02);
}
void EU_SendChar(char ch) {
    //TXSTA.TXLEN = 1; //this doesnt exist
    while(TXSTAbits.TRMT == 0){
        TXREG = ch;
    }
    //while(TXSTAbits.TRMT == 0);
    //TXREG = ch;
}
unsigned char closeExteriorDoorSent(void){
    enablers |= 0x04; // bit 2
    return (confirmedEnablers & 0x04); //returns 0x04 once done 
}

unsigned char enterPinSent(void){
    enablers |= 0x08; // bit 3
    return (confirmedEnablers & 0x08); //returns 0x08 once done
}

unsigned char permissionDeniedSent(void){
    enablers |= 0x10; // bit 3
    return (confirmedEnablers & 0x10); //returns 0x0A once done
}

static unsigned char sendMessage(const char *msg, unsigned char *index, unsigned char bit){
    if(msg[*index] != '\0'){ // if there are characters left to send
        if(TXSTAbits.TRMT == 1){ // if the eusart hardware is ready 
            TXREG = msg[*index]; //so send the current char and increment the index
            (*index)++;
        }
        return 0; // to know its still sending
    }
    else{ // if nothing left to send
        *index = 0; // reset the index for next part of message
        // ~  is the bitwise NOT operator. It flips every bit in a byte
        enablers &= ~bit; // to clear the reset bit
        confirmedEnablers |= bit; // to set the confirmation bit
        return 1; // the job is done
    }
}

static unsigned char sendMessage2(const char *msg, unsigned char *index, unsigned char bit){
    if(msg[*index] != '\0'){ // if there are characters left to send
        if(TXSTAbits.TRMT == 1){ // if the eusart hardware is ready 
            TXREG = msg[*index]; //so send the current char and increment the index
            (*index)++;
        }
        return 0; // to know its still sending
    }
    else{ // if nothing left to send
        *index = 0; // reset the index for next part of message
        // ~  is the bitwise NOT operator. It flips every bit in a byte
        enablers2 &= ~bit; // to clear the reset bit
        confirmedEnablers2 |= bit; // to set the confirmation bit
        return 1; // the job is done
    }
}


void eusartMotor(void) {

    static unsigned char state = 0x00;
    static unsigned char messageIndex = 0x00;

    switch (state) {

        // STATE 0:
        // Wait for any message to be enabled
        case 0x00:
            TXSTAbits.TXEN = 0; //Disable transmission
            messageIndex = 0;
            if (enablers & 0x01) state = 0x01;      // New day message
            if (enablers & 0x02) state = 0x02;      // Exterior door open message
            if (enablers & 0x04) state = 0x03; 
            if (enablers & 0x08) state = 0x04; 
            if (enablers & 0x10) state = 0x05; 
            break;

        // STATE 1:
        // Send the "new day" message character by character
        case 0x01:
            TXSTAbits.TXEN = 1; //Enable transmission
            if(sendMessage(newDayMessage, &messageIndex, 0x01)){
                state = 0x00;
            }
        /*
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
        }*/
            break;

        // STATE 2:
        // Placeholder for future messages
        case 0x02:
            TXSTAbits.TXEN = 1; //Enable transmission
            if(sendMessage(openExteriorDoorMessage, &messageIndex, 0x02)){
                state = 0x00;
            }
            /*
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
            */
            break;
        case 0x03: 
            TXSTAbits.TXEN = 1; //Enable transmission
            if(sendMessage(closeExteriorDoorMessage, &messageIndex, 0x04)){
                state = 0x00;
            }
            break;
        case 0x04: 
            TXSTAbits.TXEN = 1; //Enable transmission
            if(sendMessage(enterPinMessage, &messageIndex, 0x08)){
                state = 0x00;
            }
            break;
        case 0x05: 
            TXSTAbits.TXEN = 1; //Enable transmission
            if(sendMessage(permissionDeniedMessage, &messageIndex, 0x10)){
                state = 0x00;
            }
            break;
        case 0x06: 
            break;
        case 0x07: 
            break;
        case 0x08:
            break; 
        case 0x09:
            break; 
        case 0x10:
            break; 
        case 0x11:
            break;
        case 0x12:
            break; 
    }
}
