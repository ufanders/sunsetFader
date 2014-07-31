/** @package 

    timers.c
    
    Copyright(c) Microchip Technology Inc. 2000
    
    Author: ARTHUR ANDERSON
    Created: AA  10/15/2009 10:50:57 AM
	Last change: AA 1/18/2011 2:02:23 PM
*/
/****************************************************************************/
/* Module:  TIMERS.C                                                        */
/*                                                                          */
/* Purpose:                                                                 */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*              S Y S T E M   I N C L U D E   F I L E S                     */
/****************************************************************************/

/****************************************************************************/
/*               L O C A L   I N C L U D E   F I L E S                      */
/****************************************************************************/
#include "HardwareProfile.h"
#include "include/timers.h"

/****************************************************************************/
/*                 M A C R O   D E F I N I T I O N S                        */
/****************************************************************************/
//#define USE_TIMER_4
//#define USE_TIMER_2
#define USE_TIMER_0

#if defined(USE_TIMER_4)
    #define TIMER_REG_VALUE     TMR4
    #define TIMER_REG_PERIOD    PR4
    #define TIMER_REG_CON       T4CON
    #define TIMER_INT_FLAG      IFS1bits.T4IF
    #define TIMER_INT_ENABLE    IEC1bits.T4IE
    #define TICKS_2MS           (GetPeripheralClock()/500.0)
    #define LATENCY             20
#elif defined(USE_TIMER_2)
    #define TIMER_REG_VALUE     TMR2
    #define TIMER_REG_PERIOD    PR2
    #define TIMER_REG_CON       T2CON
    #define TIMER_INT_FLAG      PIR1bits.TMR2IF
    #define TIMER_INT_ENABLE    PIE1bits.TMR2IE
    #define TICKS_100MS         (GetInstructionClock()/10.0)
    #define TIMER_PRESCALE
#else
    #define TIMER_REG_VALUE     TMR0
    #define TIMER_REG_CON       T0CON
    #define TIMER_INT_FLAG      INTCONbits.T0IF
    #define TIMER_INT_ENABLE    INTCONbits.T0IE
    #define TICKS_100MS         (GetInstructionClock()/10.0)
    #define LATENCY             20
    #define TIMER_PRESCALE      256
    #define TIMER_RELOAD   (int)(0xFFFF - (TICKS_100MS - LATENCY)/TIMER_PRESCALE)
#endif


/****************************************************************************/
/*                  T Y P E   D E F I N I T I O N S                         */
/****************************************************************************/

/****************************************************************************/
/*        A P P L I C A T I O N   S C O P E   D E C L A R A T I O N S       */
/*                                                                          */
/*  These should be paired with an 'extern' statement in an included header */
/****************************************************************************/


/****************************************************************************/
/*             M O D U L E   S C O P E   D E C L A R A T I O N S            */
/****************************************************************************/
volatile unsigned int Timer_SysTime;
volatile unsigned int now;

/****************************************************************************/
/*                          F U N C T I O N S                               */
/****************************************************************************/

/****************************************************************************/
/* Function:    Constructor                                                 */
/*                                                                          */
/* Initialises the software timer system                                    */
/*                                                                          */
/****************************************************************************/

void Timer_Constructor ( void )
{
    Timer_SysTime = 0;

    //TIMER_REG_PERIOD = TICKS_100MS/TIMER_PRESCALE

    //we update TMR0H first because both TMR0H and TMR0L get updated
    //upon a write to TMR0L
    TMR0H = (TIMER_RELOAD >> 8) & 0xff;
    TMR0L = TIMER_RELOAD & 0xff;

    TIMER_REG_CON = 0b10000111; // Timer on, 16-bit, TCY, Prescale 1:256
    TIMER_INT_FLAG = 0; // but dismiss any spurious interrupts
    TIMER_INT_ENABLE = 1; // Enable Timer Interrupt
}
/****************************************************************************/
/* Function:    Timer_Start                                                 */
/*                                                                          */
/* Start a timer of the specified length                                    */
/*                                                                          */
/****************************************************************************/

void _Timer_Start ( Timer_t *theTimer, unsigned int length )
{
    TIMER_INT_ENABLE = 0;                 // Disble Timer Interrupt
    theTimer->started = Timer_SysTime;
    TIMER_INT_ENABLE = 1;                 // Enable Timer Interrupt

    if (length == 1)
    {
        length++; //ensure first expiration is not short
    }
    
    theTimer->length = length;
}

/****************************************************************************/
/* Function:    Timer_IsExpired                                             */
/*                                                                          */
/* Returns TRUE if the provided timer has expired.                          */
/*                                                                          */
/****************************************************************************/

unsigned char _Timer_IsExpired ( Timer_t *theTimer )
{
    int length = theTimer->length;

    if ( length != 0 )
    {
        TIMER_INT_ENABLE = 0;                 // Disble Timer Interrupt
        now = Timer_SysTime;
        TIMER_INT_ENABLE = 1;                 // Enable Timer Interrupt

        now -= theTimer->started;
        return (now >= length);
    }

    return 0;
}

/****************************************************************************/
/* Function:    Timer_Stop                                                  */
/*                                                                          */
/* Stops a timer, so that it no longer reports as Expired                   */
/*                                                                          */
/****************************************************************************/
#if 0
// Done as macro
void Timer_Stop ( Timer_t *theTimer )
{
    theTimer->length = 0;
}
#endif
/****************************************************************************/
/* Function:    Timer_IsActive                                              */
/*                                                                          */
/* Returns TRUE if the timer is not stopped.                                */
/* An expired timer will report as active.                                  */
/*                                                                          */
/****************************************************************************/
#if 0
// Done as macro
unsigned char Timer_IsActive ( Timer_t *theTimer )
{
    return theTimer->length != 0;
}
#endif

/****************************************************************************/
/***********************INTERRUPT LEVEL ROUTINE******************************/
/****************************************************************************/
/* Function:    TimerSystem::TickIsr                                        */
/*                                                                          */
/* Handle a tick interrupt from the h/w timer                               */
/*                                                                          */
/****************************************************************************/
/***********************INTERRUPT LEVEL ROUTINE******************************/
/****************************************************************************/

void TimerSystem_TickIsr ( void )
{
    Timer_SysTime++;

#if defined(USE_TIMER_4)
    //we needn't do anything because the timer value reg auto-resets
#elif defined(USE_TIMER_2)
    //we needn't do anything because the timer value reg auto-resets
#else
    //we update TMR0H first because both TMR0H and TMR0L get updated
    //upon a write to TMR0L
    TMR0H = (TIMER_RELOAD >> 8) & 0xff;
    TMR0L = TIMER_RELOAD & 0xff;
#endif

    TIMER_INT_FLAG = 0; // Re-enable the interrupt
}


