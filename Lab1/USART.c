/*
 * USART.c
 *
 *  Created on: Jan 19, 2017
 *      Author: Rayyan Khan, Ben Titus, Ryan Wiesenberg
 */

#include "../RBELib/RBELib.h"
//#include <avr/io.h>

/**
 * @brief Initializes USART1 as a print terminal to the PC. This function
 * must check the incoming baudrate against the valid baudrates
 * from the data-sheet. If the baudrate is invalid, then the
 * DEFAULT_BAUD constant must be used instead.
 *
 * @param baudrate The desired baudrate to set for USART1.
 *
 * @todo Create the function that will initialize USART1 for debugging use.
 */
void debugUSARTInit(unsigned long baudrate) {

	//check whether incoming baudrate is a valid baudrate
	switch (baudrate) {
	case 2400:   case 4800:    case 9600:   case 14400:
	case 19200:  case 28800:   case 38400:  case 57600:
	case 76800:	 case 115200:  case 230400: case 250000:
	case 500000: case 1000000: break;

	default:
		baudrate = DEFAULT_BAUD;
		break;
	}

	/*Set baud rate */
	UBRR0H = (unsigned char) (baudrate >> 8);
	UBRR0L = (unsigned char) baudrate;
	/*Enable receiver and transmitter*/

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

/**
 * @brief Sends one byte to the USART1 Tx pin (Transmits one byte).
 *
 * @param byteToSend The byte that is to be transmitted through USART1.
 *
 * @todo Make the function that will put a character on the USART1 TX line.
 */
void putCharDebug(char byteToSend) {

}

/**
 * @brief Recieves one byte of data from the serial port (i.e. from the PC).
 *
 * @return byteReceived Character that was received on the USART.
 *
 * @todo Make the function that will listen for input on the USART1 RX line.
 */
unsigned char getCharDebug(void) {

}

