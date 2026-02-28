/* 
 * File:   ADT_PUSH0.h
 * Author: waven
 *
 * Created on February 24, 2026, 12:25 PM
 */

#ifndef ADT_PUSH0_H
#define	ADT_PUSH0_H

#ifdef	__cplusplus
extern "C" {
#endif

#define TRUE 1
#define FALSE 0

void Push0_Init (void);
// Post: Initializes the ADT. 
    
void Push0_Motor (void);
// Post: Motor of the ADT.

unsigned char Push0_ThereIsPressing (void);
// Post: Returns TRUE if the pushbutton has been pressed, otherwise returns FALSE. Destructive reading. 

void Push0_End (void);
// Destroys the ADT



#ifdef	__cplusplus
}
#endif

#endif	/* ADT_PUSH0_H */

