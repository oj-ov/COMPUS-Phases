#ifndef ADT_OUT_H
#define ADT_OUT_H

void OUT_Init(void);

void OUT_LedOkOn(void);
void OUT_LedOkOff(void);

void OUT_LedAlarmOn(void);
void OUT_LedAlarmOff(void);

void OUT_LedIntensityOn(void);
void OUT_LedIntensityOff(void);
//void OUT_LedIntensityUpdate(unsigned long elapsed); 

unsigned char OUT_HallDetected(void);
unsigned char OUT_ExitPressed(void);

void OUT_Motor(void);

#endif