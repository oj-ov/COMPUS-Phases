#include "pic18f4321.h"
#include "xc8.h"

#pragma config OSC = HS
#pragma config WDT == OFF
#pragma config PBADEN = DIG

static void __interrupt(high priority) MyRSI(void);
void main(void);


void __interrupt(high priority) MyRSI(void);{
    if(TMR0IF == 1){
        TMR0IF = 0;
    }
} 

void main(void){
    TI_Init();
    LS_Init();
    while(1){
        LSMotor();
    }
}

