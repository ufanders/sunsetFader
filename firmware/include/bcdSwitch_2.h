/* 
 * File:   bcdSwitch.h
 * Author: UFAnders
 *
 * Created on March 11, 2013, 4:20 PM
 */

#ifndef BCDSWITCH_H
#define	BCDSWITCH_H

#include "../Microchip/include/Compiler.h"
#include "../Microchip/include/GenericTypeDefs.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define BCDSWITCH_DEBOUNCE_TICKS 10 //1000ms debounce time

void BcdSwitchInit(void);
void BcdSwitchGetUpdate(void);
UINT8 BcdSwitchDidChange(void);
UINT16 BcdSwitchGetDebouncedState(void);
void BcdSwitchClearChange(void);
UINT16 BcdSwitchGetValue(void);
void ShiftDBNC(UINT8, UINT8);

#ifdef	__cplusplus
}
#endif

#endif	/* BCDSWITCH_H */

