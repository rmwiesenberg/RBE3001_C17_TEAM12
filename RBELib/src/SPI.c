/*

 * SPI.c
 *
 *  Created on: Jan 28, 2017
 *      Author: Rayyan
 */

#include <RBELib/RBELib.h>
#include <avr/io.h>

void initSPI() {
	PRR = 0;

	SPI_MISO_DDR = 0;  // PB6 MISO Input
	SPI_MOSI_DDR = 1;  // PB5 MOSI Output
	SPI_SCK_DDR = 1;   // PB7 SCK Output
	SPI_MASTER_SS = 1; // PB4 SS Output

	PORTBbits._P4 = 1; // Tie SS high to deassert

	DAC_SS_ddr = 1;    // DAC SS Output
	DAC_SS = 1;        // Tie SS high to deassert

	PORTCbits._P5 = 1;  // Accelerometer SS Output
	PORTCbits._P5 = 1; // Tie SS high to deassert


	SPCR |=   (1 << SPE)   // Enable SPI
			| (1 << MSTR)  // Set as master
			| (1 << SPR1)  // set SCK freq to...
			| (1 << SPR0); // fosc/128

	SPSR &= ~(1 << SPI2X);
}

unsigned char spiTransceive(BYTE data) {
	SPDR = data;


	// wait for interrupt flag to go low
	//happens when transmit is complete
	while (!(SPSR & (1 << SPIF)))
		;

	return SPDR;
}
