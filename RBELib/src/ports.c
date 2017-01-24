/*
 * ports.c
 *
 *  Created on: Jan 24, 2017
 *      Author: ryan
 */

#include <RBELib/RBELib.h>
#include <stdarg.h>

void setPinsDir(char port, int dir, char numPins, ...) {
	va_list valist;
	va_start(valist, numPins);

	unsigned char pins = 0;

	for (int i = 0; i < numPins; ++i) {
		int pin = va_arg(valist, int);
		pins |= 1 << pin;
	}

	va_end(valist);

	switch (port) {
	case 'A':
		if (dir)
			DDRA &= ~pins;
		else
			DDRA |= pins;
		break;
	case 'B':
		if (dir)
			DDRB &= ~pins;
		else
			DDRB |= pins;
		break;
	case 'C':
		if (dir)
			DDRC &= ~pins;
		else
			DDRC |= pins;
		break;
	case 'D':
		if (dir)
			DDRD &= ~pins;
		else
			DDRD |= pins;
		break;
	}
}

unsigned char getPinsVal(char port, char numPins, ...) {
	va_list valist;
	va_start(valist, numPins);

	unsigned char pins = 0;

	for (int i = 0; i < numPins; ++i) {
		int pin = va_arg(valist, int);
		pins |= 1 << pin;
	}

	va_end(valist);

	switch (port) {
	case 'A':
		return PINA & pins;
	case 'B':
		return PINB & pins;
	case 'C':
		return PINC & pins;
	case 'D':
		return PINC & pins;
	}
	return 0;
}

void setPinsVal(char port, int val, int numPins, ...) {
	va_list valist;
	va_start(valist, numPins);

	unsigned char pins = 0;

	for (int i = 0; i < numPins; ++i) {
		int pin = va_arg(valist, int);
		pins |= 1 << pin;
	}

	va_end(valist);

	switch (port) {
		case 'A':
			if (val)
				PINA &= ~pins;
			else
				PINA |= pins;
			break;
		case 'B':
			if (val)
				PINB &= ~pins;
			else
				PINB |= pins;
			break;
		case 'C':
			if (val)
				PINC &= ~pins;
			else
				PINC |= pins;
			break;
		case 'D':
			if (val)
				PIND &= ~pins;
			else
				PIND |= pins;
			break;
		}
}
