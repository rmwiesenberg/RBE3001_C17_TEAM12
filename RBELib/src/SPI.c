/*

 * SPI.c
 *
 *  Created on: Jan 28, 2017
 *      Author: Rayyan
 */

#include <RBELib/RBELib.h>
#include <avr/io.h>

void initSPI() {
	DDRB |=   (1 << DDB5)  // MOSI enable
			| (1 << DDB7)  // SCK enable
			| (1 << DDB4); // SS enable

	PORTB |=  (1 << PB4);  // Tie SS high to deselect

	DDRB &=  ~(1 << DDB6); // MISO enable


	SPCR |=   (1 << SPE)   // Enable SPI
			| (1 << MSTR)  // Set as master
			| (1 << SPR1)  // set SCK freq to...
			| (1 << SPR0); // fosc/128
}

unsigned char spiTransceive(BYTE data) {
	SPDR = data;


	// wait for interrupt flag to go low
	//happens when transmit is complete
	while (!(SPSR & (1 << SPIF)))
		;

	return SPDR;
}
