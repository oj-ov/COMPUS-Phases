#include <xc.h>
#include "pic18f4321.h"

#include "ADT_TIMER.h"
#include "ADT_KEYSCAN.h"

#define KY_AST 10
#define KY_HASH 11

static unsigned char KS_State = 0; // for the states. 
static unsigned char KS_debtimer = 0;
static unsigned char KS_rawKey = 0; // takes the scanned value of the  key
static unsigned char KS_lastKey = 0;
static unsigned char KS_newKeyFlag = 0;

static const unsigned char ks_pos[4][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {KY_AST, 0,KY_HASH}};
//private function
static unsigned char KS_Scan(void){
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 1;
    if(PORTBbits.RB6 == 0) return ks_pos[0][0];
    if(PORTBbits.RB7 == 0) return ks_pos[0][1];
    if(PORTCbits.RC1 == 0) return ks_pos[0][2];

    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 1;
    if(PORTBbits.RB6 == 0) return ks_pos[1][0];
    if(PORTBbits.RB7 == 0) return ks_pos[1][1];
    if(PORTCbits.RC1 == 0) return ks_pos[1][2];

    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 1;
    if(PORTBbits.RB6 == 0) return ks_pos[2][0];
    if(PORTBbits.RB7 == 0) return ks_pos[2][1];
    if(PORTCbits.RC1 == 0) return ks_pos[2][2];
    
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 0;
    if(PORTBbits.RB6 == 0) return ks_pos[3][0];
    if(PORTBbits.RB7 == 0) return ks_pos[3][1];
    if(PORTCbits.RC1 == 0) return ks_pos[3][2];

    //so if no key: all rows back to high
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 1;

    return 0xFF;
}

void KS_Init(void){
    //rows = output, col as input
    //rows
    TRISBbits.TRISB2 = 0;
    LATBbits.LATB2 = 1;
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3 = 1;
    TRISBbits.TRISB4 = 0;
    LATBbits.LATB4 = 1;
    TRISBbits.TRISB5 = 0;
    LATBbits.LATB5 = 1;
    //cols
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    TRISCbits.TRISC1 = 1;
    //set to digital
    ADCON1 = 0X0F;
    //create virtual timer for the debounces
    TI_NewTimer(&KS_debtimer);
    //initial to start by having everything 0
    KS_State = 0;
    KS_rawKey = 0;
    KS_lastKey = 0;
    KS_newKeyFlag = 0;
}

unsigned char KS_hasNewKey(void){
    return KS_newKeyFlag;       
}

unsigned char KS_getLastKey(void){
    KS_newKeyFlag = 0;
    return KS_lastKey;
}

void KS_Motor(void){
    unsigned char temp;
    switch(KS_State){
        case 0: // IDLE state, keep on scanning for key press.
            KS_rawKey = KS_Scan();
            if(KS_rawKey != 0xFF){ // if the key press detected
                TI_ResetTics(KS_debtimer); // start debounce
                KS_State = 1; // KS_State++;
            }
        break;
        case 1://debounce wait for 16 ms
            if(TI_GetTics(KS_debtimer) >= 8){ // wait till debounce ends
                KS_State = 2;
                //KS_lastKey = 1;
            }
        break;
        case 2: /* confirm if the new reading is same/different as the previous one.
        Use temp variable to store and compare.*/
            temp = KS_Scan(); // for new scan, if the last one gets lost? eaiser to compare
            //KS_rawKey = KS_Scan();
            if(temp == KS_rawKey && temp != 0xFF){
                // here if the same key stil pressed and real key are same values
                KS_lastKey = KS_rawKey; //store it to the last updated key
                KS_newKeyFlag = 1; // to say that new key entry is possible
                KS_State = 3;
            }
            else{
                KS_State = 0; // key gone or different key, start over from State 0
            }
        break;
        case 3:// dont start taking values again until the key is unpressed
            if(KS_Scan() == 0xFF){
                KS_State = 0;
            }
        break;
    }
}