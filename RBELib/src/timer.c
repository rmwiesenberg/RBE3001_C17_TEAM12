/*
 * timer.c
 *
 *  Created on: Jan 23, 2017
 *      Author: Ryan
 */

#include <RBELib/RBELib.h>

static const int prescale = PRESCALE1;

void initTimer(unsigned char timer, int mode, unsigned char comp){
	cli(); // disable interrupts until setup is finished
	switch(timer){
	case 0:
		TCCR0A = COM0A0 << mode | WGM00 << mode;
		TCCR0B = CS00 << prescale;
		OCR0A = comp;
		TIMSK0 |= OCIE0A << 1;
		break;
	case 1:
		PORTC ^= 0x02;
		TCCR1A = mode << COM1A0;
		TCCR1B = prescale << CS10 | 1 << WGM12;
		OCR1AL = comp;
		TIMSK1 |= 1 << OCIE0A;
		break;
	case 2:
		TCCR2A = COM2A0 << mode | WGM20 << mode;
		TCCR2B = CS20 << prescale;
		OCR2A = comp;
		TIMSK2 |= OCIE2A << 1;
		break;

	default:
		break;
	}
	sei();
}

void setCompValue(unsigned char timer, unsigned char comp){
	switch(timer){
	case 0:
		OCR0A = comp;
		break;
	case 1:
		OCR1A = comp;
		break;
	case 2:
		OCR1B = comp;
		break;
	}
}


