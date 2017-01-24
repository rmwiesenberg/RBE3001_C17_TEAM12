#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define POT_CHANNEL 4

unsigned char mode, tog;
int maxCount;

void potRead() {

	DDRB = 0xFF; //Set Port as output
	initADC(POT_CHANNEL);		// init ADC on port 4
	while (1) {
		printf("Input a command character: \n\r");
		char cmd = getCharDebug();			// polls for input, locks up program
		if (cmd == 'S') {

			for (int i = 0; i < 250; i++) {
				unsigned int adcVal = getADC(POT_CHANNEL);// implemented in adc.c

				int potOut_mVolts = (int) (adcVal * (5000. / 1023.));
				int potAngle = (int) (adcVal * (300. / 1023.));

				printf("%d,%d,%d,%d\n\r", i, adcVal, potOut_mVolts, potAngle);

			}
			printf("DONE \n\r");

		} else {
		}
	}
}

void initTimerFreq() {
//	DDRB &= 0x00;
	DDRC |= 0x02;
	PORTC &= 0xFD;

	//initialize timer to 1Hz
	initTimer(1, 0, 0x50);
}

void outputWave() {
	//set timer to count up to one of three values according to pushbuttons
	//pushbuttons on sw7, sw6, sw5
	unsigned char pinB = PINB;
	switch (pinB) {
	case 0x80:
		if (mode != 1) {
			maxCount = 18000;
			setCompValue(1, maxCount);
			OCR1AH = 0x46;
			mode = 1;
		}
		break;

	case 0x40:
		if (mode != 2) {
			maxCount = 900;
			setCompValue(1, maxCount);
			mode = 2;
		}
		break;

	case 0x20:
		if (mode != 3) {
			maxCount = 180;
			setCompValue(1, maxCount);
			mode = 3;
		}
		break;

	default:

		break;
	}

	//set the output high when timer1 resets
	if (tog) {
		tog = 0;
		PORTC |= 0x01;
	}

	//read potentiometer and timer values
	unsigned short potVal = getADC(POT_CHANNEL);
	unsigned short timerVal = TCNT1L;
	timerVal = TCNT1H << 8;

	//set pwm according to potentiometer
	if (timerVal > (maxCount>>1)) {
		PORTD &= 0xFE;
	}
}

ISR(TIMER1_COMPA_vect) {
	tog = 1;
}


int main(void) {
	initRBELib();
	debugUSARTInit(115200);
	potRead();

	return 0;
}
