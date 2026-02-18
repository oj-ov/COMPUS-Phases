#include "ADT_EUSART.h"
unsigned char enablers = 0x00; //Variable to enable the different messages to be sent, each bit is for a different message. For example, bit 0 is for the new day message, bit 1 for the new hour message, etc.
//BIT 0: Enable new day message

unsigned char confirmedEnablers = 0x00;

unsigned static char newDayMessage[20] = "New day has started!";

unsigned char newMessageSent(void);
void EusartMotor(void);

unsigned char newMessageSent(){
    enablers = 0x01;
    return confirmedEnablers & 0x01;
}

void EusartMotor(){
    static unsigned char state = 0x00;
    static unsigned char messageIndex = 0x00;

    switch(state){

        //STATE 0:
        //Wait for the main controller to enable the new day message being sent, and then send it
        case 0x00:
            if(enablers & 0x01){ //If the new day message is enabled
                 state = 0x01;
            }
            if(enablers & 0x02){ //If the new hour message is enabled
                state = 0x02;
            }
        break;

        //STATE 1:
        case 0x01:
            if(messageIndex < 20){ //If there are still characters to be sent
                if(TXSTAbits.TRMT == 0){
                    TXREG = newDayMessage[messageIndex]; //Send the next character
                    messageIndex++;
                }
            }
            else{ //If the message has been sent, reset the variables and go back to state 0
                messageIndex = 0x00;
                enablers &= 0xFE; //Disable the new day message
                confirmedEnablers |= 0x01; //Confirm that the new day message has been sent
                state = 0x00;
            }
        break;

        case 0x02:
        break;

    }

}