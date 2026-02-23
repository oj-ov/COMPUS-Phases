#include <xc.h>
#include "MAIN_CONTROLLER_ADT.h"
#include "ADT_EUSART.h"
#include "ADT_TIMER.h"
#include "ADT_SPEAKER.h" 
#include "ADT_KEYSCAN.h"
#include "ADT_KEYSMS.h"
#include <string.h>

#define TWO_SECONDS 2000
#define PIN_LENGTH 7
#define MAX_ATTEMPTS 3
//UL is unsigned long, so only to have psotive range
#define MAX_TIMEOUT 120000UL //2 mins
#define WARN_FAST 105000UL //1:45 mins
static char pin[PIN_LENGTH + 1]; // as it is 8 bits
static unsigned char pin_index = 0;
static unsigned char pin_attempts = 0;
static const char correct_pin[PIN_LENGTH + 1] = "1611MON";
static unsigned char pin_timer = 0;
static unsigned char beep_timer = 0;
unsigned char timer0 = 0;

void MainControllerMotor(void) {

    static unsigned char state = 0x00;

    switch (state) {

        // STATE 0:
        // Send the "new day" message to the PC screen
        case 0x00:
            /* Calling the EUSART ADT to enable its message being sent and then
             * waiting for it to be sent before changing the state
             */
            pin_attempts = 0;
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
            if(TI_GetTics(timer0) >= TWO_SECONDS){
                TI_ResetTics(timer0);
                state = 0x05;
            }
            break;
        //State 5
        case 0x05:// close the door + beep sound from speaker.
            if(closeExteriorDoorSent() == 0x04){
                SP_BeepHigh();
                state = 0x06;
            }
            break;
        case 0x06: // enter pin and init pin timer
                if(enterPinSent() == 0x08){
                    pin_index = 0;
                    memset(pin, 0, sizeof(pin)); // used to fill a contiguous block of memory with a specific value
                    TI_NewTimer(&pin_timer);
                    TI_ResetTics(pin_timer);
                    TI_NewTimer(&beep_timer);  
                    TI_ResetTics(beep_timer); 
                    state = 0x07;
                }
        break;
        case 0x07:{
            KS_Motor();
            KSMS_Motor();
            unsigned long time_elapse = TI_GetTics(pin_timer);
            // based in the different sound timings
            unsigned long beep_interval = (time_elapse < WARN_FAST) ? 1000UL : 500UL; 
            if (TI_GetTics(beep_timer) >= beep_interval) {
                TI_ResetTics(beep_timer);
                SP_BeepLow();
            }
            
            // to recieve characters
            if(KSMS_hasNewChar()){
                char ch = KSMS_getLastChar();
                pin[pin_index++] = ch;
                EU_SendChar(ch);                
            }
            if(TI_GetTics(pin_timer) >= MAX_TIMEOUT){
                state = 0x10;
                break;
            }
            if(pin_index >= PIN_LENGTH){
                pin[PIN_LENGTH] = '\0';
                state = 0x08;
            }
            break;
        }
        case 0x08:// validate pin
            if(strcmp(pin,correct_pin) == 0){
                pin_attempts = 0;
                pin_index = 0; // reset the space
                LATAbits.LATA4 = 0; // turn of intensity led
                state = 0x0A; // opening the interior door.
            }
            else{ // if pin is wrong
                pin_attempts++;
                pin_index = 0; // reset the space
                memset(pin, 0, sizeof(pin));
                if (pin_attempts >= MAX_ATTEMPTS || TI_GetTics(pin_timer) >= MAX_TIMEOUT) {
                    state = 0x10; // alarm for too many attempts.
                }
                else{
                    state = 0x09; // permission denied then retry
                }
            }
        break;
        case 0x09: // retry
                if(permissionDeniedSent() == 0x10){
                    state = 0x06; // re enter the pin.
                }
        break;
        case 0x0A: 
            if (openInteriorDoorSent() == 0x20) {
                TI_ResetTics(timer0);
                state = 0x0B;
            }
            break;
        case 0x0B: 
            if(TI_GetTics(timer0) >= TWO_SECONDS){
                state = 0x0C;
            }
            break;
        case 0x0C: 
            if(closeInteriorDoorSent() == 0x40){
                SP_BeepHigh();
                state = 0x0D;
            }
            break;

        default:
            // Optional: handle unexpected states
            state = 0x00;
            break;

    }
}