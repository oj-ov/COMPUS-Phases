#include "pic18f4321.h"
#include "xc8.h"

//Other ADTs
#include "ADT_TIMER.h"
#include "MAIN_CONTROLLER_ADT.h"

#pragma config OSC = HS //10MHz crystal
#pragma config WDT == OFF
#pragma config PBADEN = DIG

static void __interrupt(high priority) MyRSI(void);
void main(void);

void __interrupt(high priority) MyRSI(void);{
    if(TMR0IF == 1){
        TMR0IF = 0;
    }
}

void initPorts(){
    //Initialize the Eusart ports
    TRISCbits.TRISC6 = 1; //TX pin as input (the system automatically changes it to output when we write something to the TXREG)
    TRISCbits.TRISC7 = 1; //RX pin as input
}

void initInterrupts(){
    RCONbits.IPEN = 1; //Enable priority levels on interrupts
    //The timer0 ADT already initializes the timer0 interrupt, so we don't need to do it here.
}


void main(void){
    //initOscillator(); The oscillator is already @ 10MHz, so we don't need to initialize it.
    initPorts();
    initInterrupts();

    //Initialize the timer ADT for all possible timers
    TI_Init();

    //LS_Init();
    while(1){
        //LSMotor();
        MainControllerMotor(); 
    }
}

