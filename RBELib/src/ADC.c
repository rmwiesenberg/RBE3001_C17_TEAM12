/*
 * ADC.c
 *
 *  Created on: Jul 14, 2014
 *      Author: ewillcox
 */

#include "RBELib/RBELib.h"

/*
 * 50khz - 200khz
 * Free running - sample constantly
 * 13 cycles a conversion - 25 1st
 * P 246 - proper changing of channels
 *
 * ADC = (VIN * 1024) / Vref
 */
void initADC(int channel){
  //Bits 7 - 6: Coupling capacitor at AREF
  //Bit 5: No left adjustment
  //Bits 4 - 0: Channel selection (0 - 7 for single ended)
  ADMUX = (0x40) | channel;

  //Bit 7: Enable ADC
  //Bit 6: Starts conversions
  //Bit 5: Auto trigger enable
  //Bit 4: Interrupt flag telling conversions are complete
  //Bit 3: Interrupt enable
  //Bits 2 - 0: Prescaler of 128 (18432000 / 128 = 144kHz)
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

  ADCSRA |= (1 << ADEN);  // Enable ADC
  ADCSRA |= (1 << ADSC);  // Start A2D Conversions
  //ADCSRA = 0xFF;

  //Bit 7: Reserved
  //Bit 6: Analog Comparator Multiplexer Enable (ACME) - leave at 1
  //Bit 5 - 3: Reserved
  //Bit 2 - 0: Free running mode
  ADCSRB = 0x40;

  //Testing with port 7
  DDRA = 0x00;
}

void clearADC(int channel)
{
	//untested, BAsically never needed
	  ADMUX = (0x40) & channel;
}

unsigned short getADC(int channel)
{
// select the corresponding channel 0~7
// ANDing with '7' will always keep the value
// of 'channel' between 0 and 7
channel &= 0b00000111;  // AND operation with 7
ADMUX = (ADMUX & 0xF8)|channel;     // clears the bottom 3 bits before ORing

// start single conversion
// write '1' to ADSC
ADCSRA |= (1<<ADSC);

// wait for conversion to complete
// till then, run loop continuously
while(!(ADCSRA & (1<<ADIF)));
//Clear ADIF by writing one to it
ADCSRA|=(1<<ADIF);

return (ADC);
}


void changeADC(int channel){
  //Change the channel using the same setting from initADC()
  ADMUX = (0x40) | channel;
}



