/*
 * USART.c
 *
 *  Created on: Jan 21, 2017
 *      Author: Rayyan Khan, Ben Titus, Ryan Wiesenberg
 */


#include "RBELib/RBELib.h"

#define F_OSC 18432000
#define CLEAR 0x00

void debugUSARTInit(unsigned long baudrate) {
	// baudrate calc on Datasheet pg. 174
	int ubrr_val = (int) (-1 + (F_OSC/(16 * baudrate)));

	unsigned char hi = (ubrr_val >> 8) & 0x0F; 	// x x x x | b11 b10 b9 b8
	unsigned char lo = ubrr_val & 0xFF;	// b7 b6 b5 b4 | b3 b2 b1 b0

	// set baud rate registers
	UBRR1H = hi;
	UBRR1L = lo;

	// set receive and transmit functionality
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);

	// set frame size (8-bit data, 1 stop bit)
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}


void putCharDebug(char byteToSend) {
	// polling for empty
	while(!(UCSR1A & (1 << UDRE1)));

	// write byte into buffer
	UDR1 = byteToSend;
}


unsigned char getCharDebug(void) {
	// polling for buffer
	while(!(UCSR1A & (1 << RXC1)));

	// read byte from buffer
	return UDR1;
}
