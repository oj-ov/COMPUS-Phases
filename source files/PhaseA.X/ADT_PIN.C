#include <xc.h>
#include "pic18f4321.h"
#include "ADT_TIMER.h"      
#include "ADT_SPEAKER.h"   
#include "ADT_EUSART.h"     
#include "ADT_KEYSCAN.h"
#include "ADT_KEYSMS.h"
#include "ADT_PIN.h"

#define PIN_LENGTH   7
#define MAX_TIMEOUT  120000UL    
#define WARN_FAST    105000UL 

static char pin[PIN_LENGTH];
static unsigned char pin_index = 0;
static unsigned char pin_attempts = 0;
static const char correct_pin[PIN_LENGTH] = "1611MON";
static unsigned char pin_timer = 0;
static unsigned long last_beep_tics = 0; 

void PIN_Init(void){
    pin_attempts = 0;
    pin_index = 0;
}

void PIN_StartEntry(unsigned char sharedEntry){
    pin_index = 0;
    pin_timer = sharedEntry;
    //TI_NewTimer(&pin_timer);
    TI_ResetTics(pin_timer);
    last_beep_tics = 0;
}

void PIN_Motor(void){
    unsigned long time_elapse = TI_GetTics(pin_timer);
    unsigned long beep_interval = (time_elapse < WARN_FAST) ? 1000UL : 500UL; 
    if ((time_elapse - last_beep_tics) >= beep_interval) {
         last_beep_tics = time_elapse;   
        SP_BeepLow();
    }
    
    // to recieve characters
    if(KSMS_hasNewChar()){
        char ch = KSMS_getLastChar();
        pin[pin_index++] = ch;
        EU_SendChar(ch);                
    }  
}

unsigned char PIN_IsComplete(void){
    return (pin_index >= PIN_LENGTH);
}
unsigned char PIN_IsTimeout(void){
    return (TI_GetTics(pin_timer) >= MAX_TIMEOUT);
}

unsigned char PIN_IsValid(void){
    unsigned char i;
    for(i = 0; i < PIN_LENGTH; i++){
        if(pin[i] != correct_pin[i]){
            return 0;
        }
    }
    return 1;
}

unsigned char PIN_GetAttempts(void){
    return pin_attempts;
}

void PIN_IncrementAttempts(void){
    pin_attempts++;
}

void PIN_ResetAttempts(void){
    pin_attempts = 0;
}

unsigned long PIN_GetElapsed(void){
    return TI_GetTics(pin_timer);
}