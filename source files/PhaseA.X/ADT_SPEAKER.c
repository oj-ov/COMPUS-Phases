#include <xc.h>
#include "pic18f4321.h"
#include "ADT_SPEAKER.h"
#include "ADT_TIMER.h"

/* so i have to use the speaker based on the needs of when to sound
so i use defines firstly and do init
*/
#define LOW_HALF_PERIOD  4 // 4ms per toggle = 500Hz, so sharp beep
#define HIGH_HALF_PERIOD 1 // 1ms per toggle
#define ALARM_HALF_PERIOD 2 // 2ms per toggle

static unsigned char sp_timer = 0;
static unsigned char sp_toggle_timer = 0;
static unsigned char sp_state = 0;
static unsigned long sp_duration = 0;
static unsigned int sp_half_period = 0;

void SP_Init(void){
    TRISCbits.TRISC3 = 0; // RC3 = output.
    LATCbits.LATC3 = 0; //at initial, as silent
    TI_NewTimer(&sp_timer);
    TI_NewTimer(&sp_toggle_timer);
    TI_ResetTics(sp_timer); // to start the timer
    TI_ResetTics(sp_toggle_timer);
    sp_state = 0;
}
/* a sharp acoustic warning is produced when doors close.
 Used in exterior door close (0x05), interior door close (0x0C), both doors close (0x0F)*/
void SP_BeepHigh(void){
    sp_duration = 200; //200 ms sharp beep
    sp_half_period = HIGH_HALF_PERIOD;
    TI_ResetTics(sp_timer);
    TI_ResetTics(sp_toggle_timer);
    sp_state = 1;
}
/*periodic low-pitched acoustic warnings" during PIN entry.
Called every 1000ms (0:00→1:45) or every 500ms (1:45→2:00) from state 0x07.
The interval between calls is controlled by Main Controller, not here.*/
void SP_BeepLow(void){
    sp_duration = 100; //100 ms low beep
    sp_half_period = LOW_HALF_PERIOD;
    TI_ResetTics(sp_timer);
    TI_ResetTics(sp_toggle_timer);
    sp_state = 1;
}
/*emitting an alarm sound for 10 seconds when alarm is triggered.
Used in: state 0x10 (too many attempts, timeout, or "No" on exit)*/
void SP_AlarmON(void){
    sp_duration = 10000;
    sp_half_period = ALARM_HALF_PERIOD;
    TI_ResetTics(sp_timer);
    TI_ResetTics(sp_toggle_timer);
    sp_state = 1;
}
// Silences the speaker immediately and returns to idle state.
void SP_OFF(void){
    LATCbits.LATC3 = 0;
    sp_state = 0;
}
// speaker has to be called every cycle using this motor
void SP_Motor(void){
    switch (sp_state){
        case 0: // idle state
            LATCbits.LATC3 = 0;
            break;
        case 1: 
            if(TI_GetTics(sp_toggle_timer) >= sp_half_period){
                TI_ResetTics(sp_toggle_timer);
                LATCbits.LATC3 ^= 1;
            }
            if(TI_GetTics(sp_timer) >= sp_duration){
                SP_OFF();
            }
            break;
    }
}