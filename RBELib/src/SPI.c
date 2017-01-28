/*

 * SPI.c
 *
 *  Created on: Jan 28, 2017
 *      Author: Rayyan
 */

#include <RBELib/RBELib.h>
#include <avr/io.h>

initSPI() {
	DDRB |= (1 << DDB5) | (1 << DDB7) | (1 << DDB4);
	DDRB &= ~(1 << DDB6);
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

unsigned char spiTransceive(BYTE data) {
	SPDR = data;

	while (!(SPSR & (1 << SPIF)));

	return SPDR;
}
