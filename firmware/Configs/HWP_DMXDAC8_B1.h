/* 
 * File:   HWP_DMXDAC_A1.h
 * Author: UFAnders
 *
 * Created on August 22, 2012, 5:23 PM
 */

#ifndef HWP_DMXDAC8_B1_H
#define	HWP_DMXDAC8_B1_H

#include "../Microchip/include/Compiler.h"

// Set configuration fuses (but only in Main.c where SET_CONFIG_FUSES is defined)
#if defined(SET_CONFIG_FUSES)

#pragma config FOSC = HSPLL //HS
#pragma config IESO = OFF
#pragma config PWRT = ON
#pragma config BOREN = OFF
#pragma config WDTPS = 256 //~1 second stall reset
#pragma config HFOFST = ON
#pragma config MCLRE = ON
#pragma config LVP = OFF

#ifdef __RELEASE
    #pragma config WDTEN = OFF
    #pragma config CP0 = ON // Code Protect
    #pragma config CP1 = ON
    //#pragma config CP2 = ON
    //#pragma config CP3 = ON
#else
    #pragma config WDTEN = OFF
    #pragma config CP0 = OFF // Code Protect
    #pragma config CP1 = OFF
    //#pragma config CP2 = OFF
    //#pragma config CP3 = OFF
#endif

#endif

// Clock frequency values
// These directly influence timed events using the Tick module.  They also are used for UART and SPI baud rate generation.
#define GetSystemClock()	(32000000ul)		// Hz
#define GetInstructionClock()	(GetSystemClock()/4)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Might need changing if using Doze modes.
#define GetPeripheralClock()	(GetSystemClock()/4)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Divisor may be different if using a PIC32 since it's configurable.

// Hardware I/O pin mappings
// LEDs
#define LED_DMX_ACT_TRIS TRISCbits.TRISC1
#define LED_DMX_ACT_LAT LATCbits.LATC1

//DMX Address switch lines
//High digit
#define SW_DMX_ADDRESS_0 PORTBbits.RB0
#define SW_DMX_ADDRESS_1 PORTBbits.RB1
#define SW_DMX_ADDRESS_2 PORTBbits.RB2
#define SW_DMX_ADDRESS_3 PORTBbits.RB3
//Middle digit
#define SW_DMX_ADDRESS_4 PORTBbits.RB4
#define SW_DMX_ADDRESS_5 PORTBbits.RB5
#define SW_DMX_ADDRESS_6 PORTAbits.RA0
#define SW_DMX_ADDRESS_7 PORTAbits.RA1
//Low digit
#define SW_DMX_ADDRESS_8 PORTAbits.RA2
#define SW_DMX_ADDRESS_9 PORTAbits.RA3
#define SW_DMX_ADDRESS_10 PORTAbits.RA4
#define SW_DMX_ADDRESS_11 PORTCbits.RC0
//Internal pullups
#define SW_DMX_ADDRESS_PU 0b00111111

//SPI
#define DAC_SPI_SCK_TRIS TRISCbits.TRISC3
#define DAC_SPI_MOSI_TRIS TRISCbits.TRISC5
#define DAC_SPI_LOAD_TRIS TRISCbits.TRISC4
#define DAC_SPI_SCK_LAT LATCbits.LATC3
#define DAC_SPI_MOSI_LAT LATCbits.LATC5
#define DAC_SPI_LOAD_LAT LATCbits.LATC4

//Jumpers
#define JP_CONFIGA_PORT PORTAbits.RA5
#define JP_CONFIGB_PORT PORTCbits.RC2
#define JP_ENABLE_PORT PORTCbits.RC6


#endif	/* HWP_DMXDAC8_B1_H */

