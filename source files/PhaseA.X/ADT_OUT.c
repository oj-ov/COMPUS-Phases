#include <xc.h>
#include "pic18f4321.h"

static unsigned char intensity_duty = 0; // 0 to 100 % brightness

void OUT_Init(void){
    TRISCbits.TRISC4 = 0; // State_OK 
    LATCbits.LATC4 = 0;
    TRISAbits.TRISA3 = 0; // alarm
    LATCbits.LATA3 = 0;
    TRISAbits.TRISA4 = 0; // intensity
    LATAbits.LATA4 = 0;
    TRISDbits.TRISD7 = 1; //hall sensor
    TRISCbits.TRISC5 = 1; // exit request button
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
    intensity_duty = 100;
}

void OUT_LedIntensityOff(){
    intensity_duty = 0;
    LATAbits.LATA4 = 0;
}
void OUT_LedIntensityUpdate(unsigned long time_elapsed){
    // 0 to 120000 ms(2 mins) -> 0 - 100%
    if(time_elapsed >= 120000UL){
        intensity_duty = 0;
    }
    else{
        intensity_duty = (unsigned char) ((time_elapsed * 100UL)/120000UL);
    }
}
unsigned char OUT_HallDetected(){
    return PORTbits.PORTD7 == 0;
}
unsigned char OUT_ExitPressed(){    
    return PORTCbits.PORT5 == 0;
}

void OUT_Motor(void){
    static unsigned char pwm_count = 0;
    pwm_count++;
    if(pwm_count >= 100){
        pwm_count = 0;
    }
    if(pwm_count < intensity_duty){
        LATAbits.LATA4 = 1;
    }
    else{
        LATAbits.LATA4 = 0;
    }
}