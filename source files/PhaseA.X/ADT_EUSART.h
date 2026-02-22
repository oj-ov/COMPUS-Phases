/*
 * File:   ADT_EUSART.h
 * Author: waven
 *
 * Created on February 18, 2026, 12:05 PM
 */

#ifndef ADT_EUSART_H
#define ADT_EUSART_H

#ifdef __cplusplus
extern "C" {
#endif

// Public function prototypes
unsigned char newMessageSent(void);
unsigned char openExteriorDoorSent(void);
void eusartMotor(void);
void EU_SendChar(char ch);

#ifdef __cplusplus
}
#endif

#endif /* ADT_EUSART_H */