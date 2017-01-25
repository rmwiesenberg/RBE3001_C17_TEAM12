/*
 * ADC.c
 *
 *  Created on: Jan 22, 2017
 *      Author: Rayyan Khan, Ben Titus, Ryan Wiesenberg
 */

#include <RBELib/RBELib.h>
#include <avr/io.h>


void initADC(int channel) {
	cli();

	DDRA &= ~(1 << channel);

	ADCSRA |= (1 << ADEN) | (0 << ADIE) | (7 << ADPS0);

	ADMUX = (1 << REFS0)|channel;

	DIDR0 |= 0 << channel;

	sei();

}


void clearADC(int channel) {
	ADCSRA &= ~(1 << ADEN);
	ADC = 0;
}


unsigned short getADC(int channel) {
	ADCSRA &= ~(1 << ADATE);
	ADMUX &= ~31;
	ADMUX |= channel;	// set channel
	ADCSRA |= (1 << ADSC); // start conversion

	while(ADCSRA & (1 << ADSC)) continue; // poll for cleared start-conversion bit

	return ADC;
}


void changeADC(int channel) {
	ADMUX &= ~31;
	ADMUX |= channel;
}
