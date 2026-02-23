#include <xc.h>
#include "pic18f4321.h"
#include "ADT_TIMER.h"
#include "ADT_KEYSMS.h"
#include <string.h>

static unsigned char ksms_State = 0;
static unsigned char ksms_timer = 0;
//array[10][5]
static const char *ksms_table[10] = {"1", "ABC2", "DEF3", "GHI4", "JKL5", "MNO6", "PQRS7", "TUV8", "WXYZ9", "0 "};
static unsigned char ksms_lastcharvalue = 0;
static unsigned char ksms_newCharFlag = 0;
static unsigned char ksms_index = 0;
static unsigned char ksms_lastdigit = 0xFF; // digit key which is currently being edited. 

void KSMS_Init(void) {
    //here we create virtual timer for for every key
    TI_NewTimer(ksms_timer); // 1s 
    ksms_State = 0;
    ksms_lastcharvalue = 0;
    ksms_newCharFlag = 0;
    ksms_index = 0; // which character inside the digits string
    ksms_lastdigit = 0xFF;
}

unsigned char KSMS_hasNewChar(void){
    return ksms_newCharFlag;       
}

char KSMS_getLastChar(void){ // no need of unsigned as its a char 
    ksms_newCharFlag = 0;
    return ksms_lastcharvalue;
}

//1st from physcial key to char, then char takes the values, then to the index,  
void KSMS_Motor(void){
    unsigned char hasKey = KS_hasNewKey();
    unsigned char key = 0;
    unsigned char digit;
    const char *string;

    if(hasKey){
        key = KS_getLastKey(); // get the key from KEYSCAN
    }
    switch (ksms_State){
        case 0: 
            if(hasKey){
                if(key == 10 || key == 11){
                    break; // break is accepted
                }
                digit = key; // 0..9
                ksms_lastdigit = digit;
                ksms_index = 0; // first char in the string 
                TI_ResetTics(ksms_timer);
                ksms_State = 1;
            }
        break;
        case 1:
            if(hasKey){ // new key pressed during editing.
                if(key == 10 || key == 11){
                    break; // State = 0;
                }
                digit = key; // 0..9
                unsigned long time_elapsed = TI_GetTics(&ksms_timer); // how long since last pressed.
                if((digit == ksms_lastdigit) && (time_elapsed < 1000)){ 
                    // Same digit pressed quickly -> cycle current character
                    string = ksms_table[digit];
                    //ksms_table[digit] is the SMS string for that digit (like  "ABC2" ).
                    ksms_index++;
                    if(ksms_index >= strlen(string)){
                        ksms_index = 0; // for cyclic mode
                    }
                    TI_ResetTics(ksms_timer);
                    // stay in state 1
                }
                else{//// Different digit OR >=1 second since last digit press
                    if(ksms_lastdigit != 0xFF){
                        // first to finalize the previous character, that is why we use lastdigit.
                        string = ksms_table[ksms_lastdigit];
                        ksms_lastcharvalue = string[ksms_index];
                        ksms_newCharFlag = 1;
                    }
                    ksms_lastdigit = digit;
                    ksms_index = 0;
                    TI_ResetTics(ksms_timer);
                    //stay in state 1
                }
            }  
            else{ // no new key, but we check for timout to finalize the value
                if(TI_GetTics(&ksms_timer) >= 1000){
                    if(ksms_lastdigit != 0xFF){
                        // first to finalize the previous character, that is why we use lastdigit.
                        string = ksms_table[ksms_lastdigit];
                        ksms_lastcharvalue = string[ksms_index];
                        ksms_newCharFlag = 1;
                    }
                    ksms_lastdigit = 0xFF;
                    ksms_State = 0;
                }
            }    
        break;
    }
}





//