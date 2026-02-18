#ifndef ADT_KEYPAD_H
#define ADT_KEYPAD_H

void Init_Keypad(void);
//Pre: this functions needs to be called to initialize the keypad, sets the rows and colums as outputs and inputs.
//Post: the keypad is initialized and ready to be used. And virtual timers for debouncing and sms are created.
void KeyPadMotor(void);
//Pre: this function needs to be called in the main loop, it will check if a key is pressed and if so, it will return the value of the key.
//Post: if a key is pressed, the value of the key is stored in a variable and can be retrieved with the getValue function, if no key is pressed, the function does nothing
unsigned char getValue(char *ch);
//Pre: this function needs to be called after a key is pressed, it will return the value of the key that was pressed.  
//Post: the value of the key that was pressed is returned, if no key was pressed, the function returns 0.
#endif // ADT_KEYPAD_H