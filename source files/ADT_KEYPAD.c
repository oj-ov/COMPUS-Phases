#include <xc.h>
#include "pic18f4321.h"
#include "ADT_TIMER.h"
#include "ADT_KEYPAD.h"

static unsigned char kp_sms_timer;
static unsigned char kp_debounced;
static unsigned char key_value = 0;

void KeypadInit(void) {
    //ROWS(output)
    // Set RB values as output for keypad so from RB2 to RB5
    TRISBbits.TRISB2 = 0; 
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0; 
    TRISBbits.TRISB5 = 0; 
    // I set here all rows as high, so that when a key is pressed, the corresponding row will be pulled low.
    LATBbits.LATB2 = 1; // Initialize RB2 to low
    LATBbits.LATB3 = 1; // Initialize RB3 to low
    LATBbits.LATB4 = 1; // Initialize RB4 to low
    LATBbits.LATB5 = 1; // Initialize RB5 to low
    //COLS(input)
    // Set RB6, RB7 and RC1 as input for keypad columns
    TRISBbits.TRISB6 = 1; // Set RB6 as input for keypad
    TRISBbits.TRISB7 = 1; // Set RB7 as input for keypad
    TRIScbits.TRISC1 = 1; // Set RD0
    ADCON1 = 0x0F; 
    //here we create virtual timers, one for debouncing and one for the sms
    TI_NewTimer(&kp_sms_timer);
    TI_NewTimer(&kp_debounced);
}

void KeyPadMotor(void) {

}

// cause it has to compare with the upcoming values of the choice entered, so ot keeps on checking with that one.
char getValue(char *ch) {
    if(key_value != 0){
        *ch = key_value; // trasnfer the value of the key to the variable pointed by ch
        key_value = 0; // to reset the key value
        return 1;
    }
    return 0;
}

