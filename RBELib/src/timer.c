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
		TCCR0A = mode << COM0A0 | mode << WGM00;
		TCCR0B = prescale << CS00;
		TIMSK0 |= 1 << OCIE0A;
		OCR0A = comp;

		if(mode == 1) {
			TCCR0A |= 1 << COM0B1;
			TCCR0B |= 1 << WGM02;
			DDRB |= 1 << DDRB4;
		}
		break;
	case 1:
		TCCR1A = mode << COM1A0 | (mode*mode) << WGM10;
		TCCR1B = prescale << CS10 | 1 << WGM12;
		TIMSK1 |= 1 << OCIE1A;
		OCR1AL = comp;

		if(mode == 1){
			TCCR1A |= 1 << COM1B1;
			TCCR1B |= 1 << WGM13;
			DDRD |= 1 << DDRD4;
		}
		break;
	case 2:
		TCCR2A = mode << COM2A0 | mode << WGM20;
		TCCR2B = CS20 << prescale;
		TIMSK2 |= OCIE2A << 1;
		OCR2A = comp;

		if(mode == 1){
			TCCR2A |= 1 << COM2B1;
			TCCR2B |= 1 << WGM22;
			DDRD |= 1 << DDRD6;
		}
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
