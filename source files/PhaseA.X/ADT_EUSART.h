#ifndef ADT_EUSART_H
#define ADT_EUSART_H

// Public function prototypes
unsigned char newMessageSent(void);
unsigned char openExteriorDoorSent(void);
void eusartMotor(void);
void EU_SendChar(char ch);
static unsigned char sendMessage(const char *msg, unsigned char *idx, unsigned char bit);

#endif
