#include <xc.h>
#include "pic18f4321.h"

void OUT_Init(void){
    TRISCbits.TRISC4 = 0; // State_OK 
    LATCbits.LATC4 = 0;
    TRISAbits.TRISA3 = 0; // alarm
    LATCbits.LATC3 = 0;
    TRISAbits.TRISA4 = 0; // intensity
    LATAbits.LATA4 = 0;

}

void OUT_LedOkOn(){
    LATCbits.LATC4 = 1;
}
void OUT_LedOkOff(){
    LATCbits.LATC4 = 0;

}
void OUT_LedAlarmOn(){
    LATCbits.LATC3 = 1;
}
void OUT_LedAlarmOff(){
    LATCbits.LATC3 = 0;    
}
void OUT_LedIntensityOn(){
    LATAbits.LATA4 = 1;
}
void OUT_LedIntensityOff(){
    LATAbits.LATA4 = 0;
}
void OUT_LedIntensityUpdate(unsigned long elapsed){
    if(elapsed < 30000UL){ // 30 seconds
        LATAbits.LATA4 = (elapsed % 4 == 0) ? 1 : 0; // 25%
    }
    else if{(elapsed < 60000UL) // 60 seconds
        LATAbits.LATA4 = (elapsed % 2 == 0) ? 1 : 0; // 50%
    }
    else if(elapsed < 90000UL){ // 90 seconds
        LATAbits.LATA4 = (elapsed % 2 == 0) ? 1 : 0; // 75%
    }
    else{
        LATAbits.LATA4 = 1; //100%
    }
}

unsigned char OUT_HallDetected(){
    return LATDbits.LATD7 == 0;
}
unsigned char OUT_ExitPressed(){    
    return LATCbits.LATC5 == 0;
}