/*
 * DAC.c
 *
 *  Created on: Jan 28, 2017
 *      Author: Rayyan Khan, Ben Titus, Ryan Wiesenberg
 */


#include <RBELib/RBELib.h>
#include <avr/io.h>

void setDAC(int DACn, int SPIVal){
	DDRD |= (1<<DDD4);
	PORTD |= (1 << PD4);

	BYTE cmdAddr = 0x30 | ((char) DACn);
	BYTE dataH = (char) (SPIVal >> 4);
	BYTE dataL = ((char) SPIVal) << 4;

	PORTD &= ~(1 << PD4);

	spiTransceive(cmdAddr);
	spiTransceive(dataH);
	spiTransceive(dataL);

	PORTD |= (1 << PD4);
	PORTD &= ~(1 << PD4);
	PORTD |= (1 << PD4);

}
