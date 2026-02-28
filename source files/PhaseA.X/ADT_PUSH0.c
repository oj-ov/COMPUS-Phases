#include <xc.h>
#include "ADT_PUSH0.h"
#include "TAD_TIMER_ENGv2.h"

#define TM50MS 25  // 50ms at 2ms/tick
#define PRESSED 1

static unsigned char handler;
static unsigned char pressed_event;

void Push0_Init(void) {
    TRISBbits.TRISB0 = 1;
    INTCON2bits.RBPU = 0;
    pressed_event = 0;
    TI_NewTimer(&handler); 
}

unsigned char Push0_ThereIsPressing(void) {
    unsigned char aux = pressed_event;
    pressed_event = 0; // Destructive read: clear after returning
    return aux;
}

void Push0_Motor(void) {
    static char state = 0;

    switch(state) {
        case 0: // IDLE
            if (PORTBbits.RB0 == PRESSED) {
                TI_ResetTics(handler);
                state = 1;
                LATAbits.LATA4 = 1;
            }
            //LATAbits.LATA4 = 1;
            break;

        case 1: // Debounce Press
            if (TI_GetTics(handler) >= TM50MS) {
                if (PORTBbits.RB0 == PRESSED) {
                    pressed_event = 1; // Trigger the event
                    state = 2;
                } else {
                    state = 0;
                }
            }
            break;

        case 2: // Wait for Release
            LATAbits.LATA4 = 1;
            if (PORTBbits.RB0 != PRESSED) {
                TI_ResetTics(handler);
                state = 3;
            }
            break;

        case 3: // Debounce Release
            if (TI_GetTics(handler) >= TM50MS) {
                state = 0; // Return to IDLE
            }
            break;
    }
}

void Push0_End(void) {
    TI_CloseTimer(handler);
}