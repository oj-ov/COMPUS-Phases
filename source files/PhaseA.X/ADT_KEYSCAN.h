#ifndef ADT_KEYSCAN_H
#define ADT_KEYSCAN_H

void KS_Init(void);
//Pre: TI_Init() has been called
//Post: internal debounce timer and state variables are initialized.
unsigned char KS_hasNewKey(void);
//Pre: KS_Init() has been called
//Post: Returns 1 if new debounced key is ready, else 0
unsigned char KS_getLastKey(void);
//Pre: KS_hasNewKey() = 1
//Post: Returns the key code, clears the new key flag
void KS_Motor(void)
//Pre: KS_Init() has been called
//Post: Scans Keypad and debounces key presses, called repeatedly in main loop

#endif
