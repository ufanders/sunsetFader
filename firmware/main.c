/*
 * File:   main.c
 * Author: UFAnders
 *
 * Created on November 7, 2012, 12:11 PM
 */

#include "Microchip/include/Compiler.h"
#include "Microchip/include/GenericTypeDefs.h"
#include "Microchip/include/TimeDelay.h"

#define SET_CONFIG_FUSES
#include "HardwareProfile.h"
#include "include/timers.h"

#define COLOR_PALETTE_SIZE 6
#define COLOR_PALETTE_INTERVAL (256/COLOR_PALETTE_SIZE)
#define COLOR_TWEEN_STEPS 10
#define COLOR_TWEEN_INTERVAL (COLOR_PALETTE_INTERVAL/COLOR_TWEEN_STEPS)

typedef struct {
   //NOTE: this must be unsigned for color tweening to work!
   signed short red, green, blue;
} led_type;

led_type led[1]; //holders for all LED color values
led_type colorPalette[COLOR_PALETTE_SIZE] =
{
    0,0,0, //off
    62,51,85, //twilight
    202,62,1, //sunrise
    253,127,4, //daybreak
    121,184,175, //skylight
    255,242,181 //noon
};

//Function Prototypes
void main(void);
void InitializeHardware(void);
UINT8 JumperStateDidChangeAny(void);
void clear_LED_array(void);
void send_LED(led_type);
void test_LEDS(void);
void sendShort(unsigned short);
void paintSunset(unsigned short);

/*
void low_isr(void);
void high_isr(void);
*/

//For PIC18 devices the low interrupt vector is found at
//00000018h. The following code will branch to the
//low_interrupt_service_routine function to handle
//interrupts that occur at the low vector.

/*
#pragma code low_vector=0x18
void interrupt_at_low_vector(void)
{
  _asm GOTO low_isr _endasm
}
#pragma code //return to the default code section

#pragma interruptlow low_isr
void low_isr (void)
{
  //...
}

//For PIC18 devices the high interrupt vector is found at
//00000008h. The following code will branch to the
//high_interrupt_service_routine function to handle
//interrupts that occur at the high vector.
 
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
  _asm GOTO high_isr _endasm
}
#pragma code //return to the default code section

#pragma interrupt high_isr
*/
void interrupt high_isr (void) //entry/exit takes 115 TCY
{

}
//#pragma code /* return to the default code section */

//Variables
UINT8 jumperStates[1];

void main(void)
{
    UINT8 resetCause;

    InitializeHardware();

    //resetCause will be populated with bits that reflect what caused the last
    //system reset. If bits[4:0] are all set, we probably had a stack
    //under/overflow.
    resetCause = RCON;
    Nop(); //instructions to break on
    Nop();
    Nop();
    RCON |= 0b10011; //deassert (set) reset causes to catch next event
	
    //init jumper/status states
    jumperStates[0] = JP_CONFIGA_PORT; //init jumper state

    paintSunset(0); //clear output

    while(1)
    {
        ClrWdt(); //clear watchdog timer

        //Background processes//

        //Application processes//
        
        if(!ADCON0bits.GODONE)
        {
            paintSunset(ADRESH);
            ADCON0bits.GODONE = 1; //begin next conversion
        }
    }
}

void InitializeHardware(void)
{
    //all pins as digital
    ANSEL = 0x00;
    ANSELH = 0x00;

    OSCCONbits.IRCF = 0b111; //16MHz

    //DMX address pins must be pulled up; address lines not attached
    //to PORTB have external pullup resistors.
    WPUB = SW_DMX_ADDRESS_PU; //enable individual PORTB pullups
    INTCON2bits.RBPU = 0; //engage PORTB pullups

    //SPI pins as output
    DAC_SPI_SCK_TRIS = 0;
    DAC_SPI_MOSI_TRIS = 0;
    DAC_SPI_LOAD_LAT = 1;
    DAC_SPI_LOAD_TRIS = 0;

    ANSELbits.ANS0 = 1; //RA0 as analog
    ADCON1bits.VCFG = 0b00; //VDD and GND as references
    ADCON0bits.CHS = 0b0000; //AN0
    ADCON0bits.ADON = 1; //ADC ON
    ADCON2bits.ADFM = 0; //left justified result
    ADCON2bits.ACQT = 0b111; //20 TAD
    ADCON2bits.ADCS = 0b011; //FRC

    ADCON0bits.ADON = 1; //start ADC

}

UINT8 JumperStateDidChangeAny(void)
{
    UINT8 stateComparisonResult, stateDidChangeAny;

    stateDidChangeAny = 0;

    //look for difference between previous state and current state
    stateComparisonResult = jumperStates[0] ^ JP_CONFIGA_PORT;
    jumperStates[0] = JP_CONFIGA_PORT; //update previous jumper state
    if(stateComparisonResult) stateDidChangeAny = 1;

    return stateDidChangeAny;
}

//Send the values in the LED array to the LED drivers
void send_LED(led_type led)
{
    unsigned short tempShort;
    
    tempShort = (led.red << 8);
    sendShort(tempShort);
    tempShort = (led.green << 8);
    sendShort(tempShort);
    tempShort = (led.blue << 8);
    sendShort(tempShort);

    DAC_SPI_LOAD_LAT = 0;
    Nop(); Nop(); Nop();
    
    DAC_SPI_LOAD_LAT = 1;
    Nop(); Nop(); Nop();

}

void sendShort(unsigned short shortToSend)
{
    unsigned char i;

    for(i=0; i<16; i++)
    {
        if(shortToSend & 0x8000)
        {
            DAC_SPI_MOSI_LAT = 1;
        }
        else
        {
            DAC_SPI_MOSI_LAT = 0;
        }
        shortToSend <<= 1;

        DAC_SPI_SCK_LAT = 1;
        Nop();
        DAC_SPI_SCK_LAT = 0;
    }
}

//zero out the LED array
void clear_LED_array(void)
{
    UINT8 i;

    for (i=0; i<10; i++)
    {
        led[i].red=0;
        led[i].green=0;
        led[i].blue=0;
    }
}

void test_LEDS(void)
{
    UINT8 i, j, k;

    led_type colorStepChange;
    UINT8 colorSteps;

    colorSteps = 10;

    clear_LED_array();

    led[0].red = colorPalette[0].red;
    led[0].green = colorPalette[0].green;
    led[0].blue = colorPalette[0].blue;

    send_LED(led[0]);

    DelayMs(1000);

    for(k=0; k < COLOR_PALETTE_SIZE-1; k++) //for each color change
    {
        //precalculate per-step change to get to next color
        colorStepChange.red = (colorPalette[k].red - colorPalette[k+1].red);
        colorStepChange.red /= colorSteps;
        colorStepChange.green = (colorPalette[k].green - colorPalette[k+1].green);
        colorStepChange.green /= colorSteps;
        colorStepChange.blue = (colorPalette[k].blue - colorPalette[k+1].blue);
        colorStepChange.blue /= colorSteps;

        for(i=0; i < colorSteps; i++) //for each interpolation step
        {
            led[0].red -= colorStepChange.red;
            led[0].green -= colorStepChange.green;
            led[0].blue -= colorStepChange.blue;
            send_LED(led[0]);

            Nop(); //something to break on

            DelayMs(100);
        }
    }
}

void paintSunset(unsigned short adcResult)
{
    unsigned char paletteIndex, tweenIndex;
    led_type colorStepChange;

    paletteIndex = adcResult / COLOR_PALETTE_INTERVAL;
    
    /*
    tweenIndex = adcResult % COLOR_TWEEN_INTERVAL;
    
    //precalculate per-step change to get to next color
    colorStepChange.red = (colorPalette[paletteIndex].red - \
            colorPalette[paletteIndex+1].red);
    colorStepChange.red /= COLOR_TWEEN_STEPS;
    colorStepChange.green = (colorPalette[paletteIndex].green - \
            colorPalette[paletteIndex+1].green);
    colorStepChange.green /= COLOR_TWEEN_STEPS;
    colorStepChange.blue = (colorPalette[paletteIndex].blue - \
            colorPalette[paletteIndex+1].blue);
    colorStepChange.blue /= COLOR_TWEEN_STEPS;

    led[0].red = (colorStepChange.red * (1+(tweenIndex-1)));
    led[0].green = (colorStepChange.green * (1+(tweenIndex-1)));
    led[0].blue = (colorStepChange.blue * (1+(tweenIndex-1)));
    */
    if(paletteIndex > (COLOR_PALETTE_SIZE -1))
    {
        paletteIndex = COLOR_PALETTE_SIZE -1;
    }

    led[0].red = colorPalette[paletteIndex].red;
    led[0].green = colorPalette[paletteIndex].green;
    led[0].blue = colorPalette[paletteIndex].blue;

    send_LED(led[0]);

    Nop(); //something to break on
}