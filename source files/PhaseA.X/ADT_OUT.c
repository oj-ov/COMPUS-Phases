#include <xc.h>
#include "pic18f4321.h"

#define TWO_MINUTES 120000UL

static unsigned char intensity_duty = 0; // 0 to 100 % brightness
static unsigned long initialPWM = TWO_MINUTES; // the time at which the LED should be turned on/off based on the duty cycle
static unsigned char led_timer;


void OUT_Init(void){
    TRISCbits.TRISC4 = 0; // State_OK 
    LATCbits.LATC4 = 0;
    TRISAbits.TRISA3 = 0; // alarm
    LATCbits.LATA3 = 0;
    TRISAbits.TRISA4 = 0; // intensity
    LATAbits.LATA4 = 0;
    TRISDbits.TRISD7 = 1; //hall sensor
    TRISBbits.TRISB0 = 1; // exit request button

    TI_NewTimer(&led_timer);
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
    TI_ResetTics(led_timer);
    intensity_duty = 100;
}

void OUT_LedIntensityOff(){
    intensity_duty = 0;
    LATAbits.LATA4 = 0;
}

unsigned char OUT_HallDetected(){
    return PORTbits.PORTD7 == 0;
}
unsigned char OUT_ExitPressed(){    
    return Push0_ThereIsPressing();
}

void OUT_Motor(void){
    static unsigned char state = 0;
    static unsigned long counter = 0;
    static unsigned long pwm_limit = TWO_MINUTES;

    //LATAbits.LATA4 is the led intensity pin
    switch(state){

        case 0:
            if(intensity_duty >= 100){
                state = 1;
                counter = 0;
                pwm_limit = TWO_MINUTES;
            }
        break;

        case 1:
            if(TI_getTics(led_timer) <= TWO_MINUTES){
                if(counter >= pwm_limit){
                    LATAbits.LATA4 ^= 1;
                    pwm_limit = (pwm_limit * 3) / 4;
                    counter = 0;
                }else{
                    counter++;
                }
            }
            else{
                intensity_duty = 0;
                state = 0;
            }
        break;

    } 
}