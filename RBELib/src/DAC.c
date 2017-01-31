///*
// * DAC.c
// *
// *  Created on: Jan 28, 2017
// *      Author: Rayyan Khan, Ben Titus, Ryan Wiesenberg
// */
//
//
//#include <RBELib/RBELib.h>
//#include <avr/io.h>
//
//void setDAC(int DACn, int SPIVal){
//	// uses 3 bytes of data to set the DAC output of one channel
//	// First byte is control and address: write to and power on/select DAC A/B/C/D
//	// Data is 12 bits long
//	// Second byte is high byte of data
//	// Third byte is low byte of data with lower nibble D/C
//
//	DDRD |= (1<<DDD4); // enable SS for DAC
//	PORTD |= (1 << DAC_SS); // write high to deselect DAC
//
//	BYTE cmdAddr = (0x30 | (DACn & 0x000F)); // first byte to send
//	BYTE dataH = (SPIVal >> 4);   // second byte to send
//	BYTE dataL = ((SPIVal & 0x000F) << 4); // third byte to send
//
//	PORTD &= ~(1 << DAC_SS); // select DAC
//
//	spiTransceive(cmdAddr); // transmit powerup signal and DAC select address
//	spiTransceive(dataH); // transmit first piece of data
//	spiTransceive(dataL); // transmit second piece of data
//
//	PORTD |= (1 << DAC_SS); // deselect DAC
////	PORTD &= ~(1 << DAC_SS);
////	PORTD |= (1 << DAC_SS);
//
//}

/*
 * DAC.c
 *
 *  Created on: Aug 24, 2016
 *      Author: joest
 */


/*
 * DAC.c
 *
 *  Created on: Jul 15, 2014
 *      Author: ewillcox
 */

#include "RBELib/RBELib.h"

void setDAC(int DACn, int SPIval){
  //We need to send out 3 "packages" for configuring the DAC
  unsigned char package1 = 0;
  unsigned char package2 = 0;
  unsigned char package3 = 0;
  //Temp value for creating the packages from the SPI value
  unsigned long temp = 0;

  //Package 1 is:
  //Write to and update (power up) DAC register n
  package1 = (0x30 | DACn);

  //If value is over our max, cap it at 4095
  if(SPIval >= 4096) SPIval = 4095;
  if(SPIval <= 0) SPIval = 0;

  //Copy the SPIval
  temp = SPIval;
  //Keep the upper 8 bits (drops bottom 4)
  temp = temp >> 4;
  //This is our second package
  package2 = temp;

  //Keep only the bottom 4 bits
  temp = SPIval & 0x000F;
  temp = temp << 4;
  //This will be our final package
  //The bottom 4 bits are don't cares
  package3 = temp;

  //Assert the DAC
  DAC_SS = 0;
  //transmit the 3 packets
  spiTransceive(package1);
  spiTransceive(package2);
  spiTransceive(package3);
  //Toggle the SS line to load and execute
  DAC_SS = 1;
  DAC_SS = 0;
  DAC_SS = 1;
}
