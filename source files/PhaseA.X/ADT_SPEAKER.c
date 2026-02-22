#include <xc.h>
#include "pic18f4321.h"
#include "ADT_SPEAKER.h"
#include "ADT_TIMER.h"

static unsigned char sp_timer = 0;
static unsigned char sp_state = 0;
static unsigned char sp_duration = 0;

void SP_Init(void){
    TRISCbits.TRISC3 = 0;
    LATCbits.LATC3 = 0;
    TI_NewTimer(&sp_timer);
    TI_ResetTics(sp_timer);
    sp_state = 0;
}

void SP_BeepHigh(void){
    
}

void SP_BeepLow(void){

}

void SP_Off(){

}

void SP_Motor(void){
    switch (sp_state){
        case 0: 
            LATCbits.LATC3 = 0;
            break;
        case 1: 
            LATCbits.LATC3 = 1;
            if(TI_GetTics(&sp_timer) >= sp_duration){
                SP_Off();
            }
        break;
    }
}