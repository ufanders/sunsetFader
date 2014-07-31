/* 
 * File:   TLV5628.h
 * Author: UFAnders
 *
 * Created on November 8, 2012, 3:31 PM
 */

#ifndef TLV5628_H
#define	TLV5628_H

#include "../HardwareProfile.h"
#include "../Microchip/include/Compiler.h"
#include "../Microchip/include/GenericTypeDefs.h"

#define SPI_DELAY Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop()
#define DAC_RANGE 1 //0: 1x vref (0-5v), 1: 2x vref (0-10v) output

#ifdef	__cplusplus
extern "C" {
#endif

void DacOutputSet(UINT8, UINT8, UINT8);
void DacOutputDataLatchSet(void);
void DacOutputDataLatchClear(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TLV5628_H */

