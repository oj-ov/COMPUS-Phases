#ifndef ADT_KEYSMS_H
#define ADT_KEYSMS_H

void KSMS_Init(void);
//Pre: TI_Init has been called
//Post: internal sms timer and state variables are initialized.
unsigned char KSMS_hasNewChar(void);
//Pre: ksms_Init() has been called
//Post: Returns 1 if 1s has passed to move to the next character.
char KSMS_getLastChar(void);
//Pre: ksms_hasNewChar() = 1
//Post: returns 1, and clears the new char flag
void KSMS_Motor(void);
//Pre: ksms_Init() has been called
//Post: Scans keypad for the char values also to get cyclical combinations

#endif // ADT_KEYSMS_H