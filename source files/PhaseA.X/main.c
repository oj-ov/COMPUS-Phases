/*
 * File:   main.c
 * Author: waven
 *
 * Created on February 18, 2026, 11:41 AM
 */

#include <xc.h>

// Other ADTs
#include "ADT_TIMER.h"
#include "MAIN_CONTROLLER_ADT.h"
#include "ADT_EUSART.h"

// CONFIG
#pragma config OSC = HS        // 10 MHz crystal
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
        INTCONbits.TMR0IF = 0;
        // TODO: Add Timer0 interrupt handling
    }
}

void initPorts(void) {
    // Initialize the EUSART ports
    TRISCbits.TRISC6 = 1;   // TX pin as input (auto-changes to output when writing TXREG)
    TRISCbits.TRISC7 = 1;   // RX pin as input
}

void initInterrupts(void) {
    RCONbits.IPEN = 1;      // Enable interrupt priority levels
    // Timer0 interrupt is initialized inside the Timer ADT
}

void main(void) {

    initPorts();
    initInterrupts();

    // Initialize the timer ADT for all possible timers
    TI_Init();

    while (1) {
        MainControllerMotor();
        eusartMotor();
    }
}