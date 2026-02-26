#include <xc.h>
#include "ADT_TIMER.h"
#include "MAIN_CONTROLLER_ADT.h"
#include "ADT_EUSART.h"
#include "ADT_KEYSCAN.h"    
#include "ADT_KEYSMS.h"     


// CONFIG
#pragma config OSC = HS // maybe hspll        // 10 MHz crystal
#pragma config WDT = OFF
#pragma config PBADEN = DIG

// Function prototypes
static void __interrupt(high_priority) MyISR(void);
void initPorts(void);
void initInterrupts(void);
void main(void);

// High-priority interrupt service routine
static void __interrupt(high_priority) MyISR(void) {

    if (INTCONbits.TMR0IF == 1) {
        RSI_Timer0();
        // TODO: Add Timer0 interrupt handling
    }
}

void initPorts(void) {
    // Initialize the EUSART ports
    TRISCbits.TRISC6 = 1;   // TX pin as input (auto-changes to output when writing TXREG)
    TRISCbits.TRISC7 = 1;   // RX pin as input
    
    TRISAbits.TRISA3 = 0;// state alarm
    TRISAbits.TRISA4 = 0;// intesity led
    TRISCbits.TRISC4 = 0;//state ok(green)
    
}

void initInterrupts(void) {
    RCONbits.IPEN = 1;      // Enable interrupt priority levels
    // Timer0 interrupt is initialized inside the Timer ADT
}

void main(void) {
    
    initPorts();
    initInterrupts();
    LATAbits.LATA4 = 0;
    // Initialize the timer ADT for all possible timers
    TI_Init();
    KS_Init();      
    KSMS_Init(); 
    PIN_Init();
    SP_Init();
    EU_Init();
    OUT_Init();
    while (1) {
        KS_Motor();
        KSMS_Motor();
        SP_Motor();
        PIN_Motor();
        MainControllerMotor();
        eusartMotor();
        OUT_Motor();
    }
}