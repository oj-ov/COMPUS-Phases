#include <xc.h>
#include "MAIN_CONTROLLER_ADT.h"
#include "ADT_EUSART.h"
#include "ADT_TIMER.h"
#include "ADT_SPEAKER.h" 
#include "ADT_KEYSCAN.h"
#include "ADT_KEYSMS.h"
#include "ADT_OUT.h"
#include "ADT_PIN.h"

#define TWO_SECONDS 2000

void MainControllerMotor(void) {

    static unsigned char state = 0;

    switch (state) {
       
        case 0: // new day message 
            /* Calling the EUSART ADT to enable its message being sent and then
             * waiting for it to be sent before changing the state
             */
            pin_attempts = 0;
            if (newMessageSent() == 0x01) {
                state = 1;
            }
            break;

        case 1: // Surveillance state_ok = on, wait Hall sensor
            OUT_LedOkOn();   
            if(LATDbits.LATD7 == 0 ){ // 1: No magnet. 0: Magnet.
                state = 2;
            }
            break;
        // STATE 2: 
        // Send the open exterior door message
        case 2:
            if(openExteriorDoorSent() == 0x02){
                state = 3;
            }
            break;
        
        // STATE 3: 
        // Create new timer to count new seconds and reset its ticks
        case 3:
            TI_NewTimer(&timer0);
            TI_ResetTics(timer0);
            state = 4;
            break;
            
        // STATE 4: 
        // Start counting the ticks for 2 seconds
        case 4:
            if(TI_GetTics(timer0) >= TWO_SECONDS){
                TI_ResetTics(timer0);
                state = 5;
            }
            break;
        //State 5
        case 5:// close the door + beep sound from speaker.
            if(closeExteriorDoorSent() == 0x04){
                SP_BeepHigh();
                state = 6;
            }
            break;
        case 6: // enter pin and init pin timer
                if(enterPinSent() == 0x08){
                    PIN_StartEntry();
                    OUT_LedIntensityOn();
                    state = 7;
                    
                }
            break;
        case 7:
            //KS_Motor();
            //KSMS_Motor();
            PIN_Motor();
            OUT_LedIntensityUpdate(TI_GetPinElapsed());
            if(PIN_IsTimeout()){
                state = 20; //alarm
                break;
            }
            if(PIN_IsComplete()){
                state = 8;
            }
            break;
        case 8:// validate pin
            if(PIN_IsValid()){
                PIN_ResetAttempts();
                OUT_LedIntensityOff();
                state = 10;
            } 
            else {
                PIN_IncrementAttempts();
                if(PIN_GetAttempts() >= MAX_ATTEMPTS || PIN_IsTimeout()){
                    state = 20;
                } else {
                    state = 9;
                }
            }
            break;
        case 9: // retry
                if(permissionDeniedSent() == 0x10){
                    state = 6; // re enter the pin.
                }
        break;
        case 10: 
            if (openInteriorDoorSent() == 0x20) {
                TI_ResetTics(timer0);
                state = 11;
            }
            break;
        case 11: 
            if(TI_GetTics(timer0) >= TWO_SECONDS){
                state = 12;
            }
            break;
        case 12: 
            if(closeInteriorDoorSent() == 0x40){
                SP_BeepHigh();
                state = 13;
            }
            break;
        case 13: 
            break;
        case 14: 
            break;
        case 15: 
            break;
        case 16: 
            break;
        case 17: 
            break;
        case 18: 
            break;
        case 19: 
            break;
        case 20: 
            break;
        case 21:
            break;
        case 22: 
            break;    

        default:
            // Optional: handle unexpected states
            state = 0x00;
            break;
        }

    }
}
