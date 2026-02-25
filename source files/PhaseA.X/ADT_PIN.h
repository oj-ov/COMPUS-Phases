#ifndef ADT_PIN_H
#define ADT_PIN_H

void PIN_Init(void);
void PIN_StartEntry(void); // reset index, start timers
void PIN_Motor(void); // collect chars, send via EUSART

unsigned char PIN_IsComplete(void); // returns 1 when 7 chars collected
unsigned char PIN_IsValid(void); // returns 1 if PIN matches
unsigned char PIN_IsTimeout(void); // returns 1 if 2min elapsed

unsigned char PIN_GetAttempts(void);
void PIN_IncrementAttempts(void);
void PIN_ResetAttempts(void);

unsigned long PIN_GetElapsed(void);

#endif
